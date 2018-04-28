

#include "parser_manager.h"
#include "symbol_table.h"
#include <vector>
#include <iostream>
#include <cassert>
#include <typeinfo>
#include <deque>
#include "syntax_error.h"
#include "scope_handler.h"
#include "hidden_var_handler.h"
#include "../intermediate_code/types.h"
#include "../intermediate_code/icode_generator.h"
#include "../intermediate_code/semantic_error.h"

using namespace std;
using namespace intermediate_code;

extern FILE *yyout;

namespace syntax_analyzer {

/**************** static variables & functions ****************/

    static scope_handler scp_handler = scope_handler();
    static hidden_var_handler hvar_handler = hidden_var_handler();
	static icode_generator icode_gen = icode_generator();

    /**
     * If the given expr is not of type expression_type::TABLE_ITEM_E, then expr is simply returned.
     * Otherwise, a quad tablegetelem is emitted from the given expr with index possible_table->index and the result is stored
     * in a hidden var.
     * e.g. "tablegetelem hidden_var, possible_table, possible_table->index", where hidden_var is the result,
     * possible_table is arg1 and possible_table->index is arg2.
     * @param table The table. Must not be NULL/nullptr
     * @param lineno The line number that triggered the possible emit of a quad
     * @return On the first case possible_table. On the second case (when the emit occurs), the hidden var where the
     * result of the emitted quad is stored.
     */
	static expr* emit_iftableitem(expr *possible_table, symbol_table &sym_table, unsigned int lineno) {
        assert(possible_table);

        //See documentation
        if(possible_table->type != expr::type::TABLE_ITEM_E) {
            return possible_table;
        } else {
            expr *result = expr::make_expr(expr::type::VAR_E);
            result->sym_entry = hvar_handler.make_new(sym_table, scp_handler, lineno);
            icode_gen.emit_quad(new quad(quad::iopcode::tablegetelem, result, possible_table, possible_table->index, lineno));
            return result;
        }
    }

    /**
     * Handles the semantic value of the "call" non-terminal symbol and its intermediate code generation.
     * Used by grammar rules "call -> ..."
     * @param func An expr that acts as the function's id (callable object).
     * @param elist A double ended queue with the parameters of the function in left-to-right order.
     * e.g.: If we had "f(a,b,c)" then elist should be "a, b, c"
     * @param lineno The line number which triggered the reduction of the rule "call -> ..."
     * @return A new expr which represents the returned value of the function and that expr is a hidden variable.
     */
    static expr* handle_call_rule(expr *func, deque<expr*> const &elist, symbol_table &sym_table, unsigned int lineno) {
        func = emit_iftableitem(func, sym_table, lineno);   //emit any dangling TABLE_ITEM_E type quads

        //emit param quads in reverse order. e.g. "param c", "param b", "param a"
        for(auto rit = elist.rbegin(); rit != elist.rend(); rit++)
            icode_gen.emit_quad(new quad(quad::iopcode::param, nullptr, *rit, nullptr, lineno));

        //emit call to function quad
        icode_gen.emit_quad(new quad(quad::iopcode::call, nullptr, func, nullptr, lineno));

        //getretval quad
        expr *result = expr::make_expr(expr::type::VAR_E);
        result->sym_entry = hvar_handler.make_new(sym_table, scp_handler, lineno);
        icode_gen.emit_quad(new quad(quad::iopcode::getretval, result, nullptr, nullptr, lineno));

        return result;
	}

	/**
	 * Handles the quad emit for the reduction of grammar rules "expr->expr arithmop expr" where arithmop
	 * is either ADD, SUB, MUL, DIV or MOD token
	 *
	 * Note that if both arg1 and arg2 are constants that can participate in an arithmetic operation, then
	 * no quad is emitted and instead the arithmetic operation is carried out. In that case, arg1 is the left operand
	 * and arg2 is the right operand. The returned expr is of constant number value.
	 *
	 * @param iopcode The intermediate opcode for the arithmetic operation. Must be one of the bellow:
	 * quad::iopcode::add,sub,mul,div,mod
	 * @param arg1 The arg1 of the quad which will be emitted. Must not be NULL/nullptr
	 * @param arg2 The arg2 of the quad which will be emitted. Must not be NULL/nullptr
	 * @lineno The line number which triggered the reduction of the grammar rule
	 * @return A new expr which represents the result expr of the operation.
	 *
	 * @throws intermediate_code::semantic_error if arg1 and arg2 cannot participate in an arithmetic operation
	 */
	static expr* handle_expr_arithmop_expr(quad::iopcode iopcode, expr *arg1, expr *arg2, symbol_table &sym_table, unsigned int lineno) {
	    assert(arg1 && arg2);

        switch(iopcode) {
            case quad::iopcode::add:
            case quad::iopcode::sub:
            case quad::iopcode::mul:
            case quad::iopcode::div:
            case quad::iopcode::mod:
                break;
            default:
                assert(false);  //Invalid iopcode
        }
        if(!arg1->can_participate_in_arithmop())
            throw semantic_error("arg1 cannot participate in arithmetic operation", lineno);
        if(!arg2->can_participate_in_arithmop())
            throw semantic_error("arg2 cannot participate in arithmetic operation", lineno);

        expr *result;
        if(arg1->type == arg2->type && arg2->type == expr::type::CONST_NUM_E) {
            long double const_val;
            long double arg1_val = arg1->const_val.number;
            long double arg2_val = arg2->const_val.number;

            switch(iopcode) {
                case quad::iopcode::add:
                    const_val = arg1_val + arg2_val;
                    break;
                case quad::iopcode::sub:
                    const_val = arg1_val - arg2_val;
                    break;
                case quad::iopcode::mul:
                    const_val = arg1_val * arg2_val;
                    break;
                case quad::iopcode::div:
                    const_val = arg1_val / arg2_val;
                    break;
                case quad::iopcode::mod:
                    const_val = (long long int)arg1_val % (long long int)arg2_val;
                    break;
                default:
                    assert(false);  //unreachable statement
            }
            result = expr::make_const_num(const_val);
        } else {
            result = expr::make_expr(expr::type::ARITHM_E);
            result->sym_entry = hvar_handler.make_new(sym_table, scp_handler, lineno);
            icode_gen.emit_quad(new quad(iopcode, result, arg1, arg2, lineno));
        }

        return result;
    }

    /**
     * Handles the quad emit for the reduction of grammar rules "expr->expr relop expr" where relop
     * is either GT, GE, LT, LE, EQ or NE token.
     *
     * Note that if both arg1 and arg2 are constants that can participate in an relational operation, then
     * no quad is emitted and instead the relational operation is carried out. In that case, arg1 is the left operand
     * and arg2 is the right operand. The returned expr is of constant bool value.
     *
     * @param iopcode The intermediate opcode for the relational operation. Must be one of the bellow:
     * quad::iopcode::if_eq, if_noteq, if_lesseq, if_greatereq, if_less or if_greater
     * @param arg1 The arg1 of the quad which will be emitted. Must not be NULL/nullptr
     * @param arg2 The arg2 of the quad which will be emitted. Must not be NULL/nullptr
     * @lineno The line number which triggered the reduction of the grammar rule
     * @return A new expr which represents the result expr of the operation.
     *
     * @throws intermediate_code::semantic_error if arg1 and arg2 cannot participate in a relational operation
     */
    static expr* handle_expr_relop_expr(quad::iopcode iopcode, expr *arg1, expr *arg2, symbol_table &sym_table, unsigned int lineno) {
        assert(arg1 && arg2);

        switch(iopcode) {
            case quad::iopcode::if_greater:
            case quad::iopcode::if_greatereq:
            case quad::iopcode::if_less:
            case quad::iopcode::if_lesseq:
            case quad::iopcode::if_eq:
            case quad::iopcode::if_noteq:
                break;
            default:
                assert(false);  //Invalid iopcode
        }
        if(!arg1->can_participate_in_relop())
            throw semantic_error("arg1 cannot participate in relational operation", lineno);
        if(!arg2->can_participate_in_relop())
            throw semantic_error("arg2 cannot participate in relational operation", lineno);

        expr *result;
        if(arg1->type == arg2->type && arg2->type == expr::type::CONST_NUM_E) {
            bool const_val;
            long double arg1_val = arg1->const_val.number;
            long double arg2_val = arg2->const_val.number;

            switch(iopcode) {
                case quad::iopcode::if_greater:
                    const_val = arg1_val > arg2_val;
                    break;
                case quad::iopcode::if_greatereq:
                    const_val = arg1_val >= arg2_val;
                    break;
                case quad::iopcode::if_less:
                    const_val = arg1_val < arg2_val;
                    break;
                case quad::iopcode::if_lesseq:
                    const_val = arg1_val <= arg2_val;
                    break;
                case quad::iopcode::if_eq:
                    const_val = arg1_val == arg2_val;
                    break;
                case quad::iopcode::if_noteq:
                    const_val = arg1_val != arg2_val;
                    break;
                default:
                    assert(false);  //unreachable statement
            }
            result = expr::make_const_bool(const_val);
        } else {
            result = expr::make_expr(expr::type::BOOL_E);
            result->sym_entry = hvar_handler.make_new(sym_table, scp_handler, lineno);

            icode_gen.emit_quad(new quad(iopcode, result, arg1, arg2, lineno), icode_gen.next_quad_label()+3);  //if relop is true, jump to assign:true
            icode_gen.emit_quad(new quad(quad::iopcode::assign, result, expr::make_const_bool(false), nullptr, lineno));    //else, assign false
            icode_gen.emit_quad(new quad(quad::iopcode::jump, nullptr, nullptr, nullptr, lineno), icode_gen.next_quad_label()+2);   //and then skip assign:true
            icode_gen.emit_quad(new quad(quad::iopcode::assign, result, expr::make_const_bool(true), nullptr, lineno)); //assign:true
        }

        return result;
    }

/************************* end *********************************/


    void_t Manage_program__stmts() {
		fprintf(yyout, "program -> stmts\n");
        return void_value;
    }
    void_t Manage_stmts__stmts_stmt() {
        fprintf(yyout, "stmts -> stmts stmt\n");
        return void_value;
    }
    void_t Manage_stmts__empty() {
        fprintf(yyout, "stmts -> <empty>\n");
        return void_value;
    }


    void_t Manage_stmt__expr_SEMICOLON() {
        fprintf(yyout, "stmt -> expr ;\n");
        hvar_handler.reset_count();
        return void_value;
    }
    void_t Manage_stmt__ifstmt() {
		fprintf(yyout, "stmt -> ifstmt\n");
        return void_value;
    }
    void_t Manage_stmt__whilestmt() {
        fprintf(yyout, "stmt -> whilestmt\n");
        return void_value;
    }
    void_t Manage_stmt__forstmt() {
        fprintf(yyout, "stmt -> forstmt\n");
        return void_value;
    }
    void_t Manage_stmt__returnstmt() {
        fprintf(yyout, "stmt -> returnstmt\n");
        return void_value;
    }
    void_t Manage_stmt__BREAK_SEMICOLON() {
        fprintf(yyout, "stmt -> break ;\n");
        return void_value;
    }
    void_t Manage_stmt__CONTINUE_SEMICOLON() {
        fprintf(yyout, "stmt -> continue ;\n");
        return void_value;
    }
    void_t Manage_stmt__block() {
        fprintf(yyout, "stmt -> block\n");
        return void_value;
    }
    void_t Manage_stmt__funcdef() {
        fprintf(yyout, "stmt -> funcdef\n");
        return void_value;
    }
    void_t Manage_stmt__SEMICOLON() {
        fprintf(yyout, "stmt -> ;\n");
        return void_value;
    }


    void_t Manage_expr__assignexpr() {
        fprintf(yyout, "expr -> assignexpr\n");
        return void_value;

	}

	/* arithop */
	expr* Manage_expr__expr_PLUS_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr + expr\n");
        return handle_expr_arithmop_expr(quad::iopcode::add, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_MINUS_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
		fprintf(yyout, "expr -> expr - expr\n");
        return handle_expr_arithmop_expr(quad::iopcode::sub, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_MUL_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr * expr\n");
        return handle_expr_arithmop_expr(quad::iopcode::mul, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_DIV_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr / expr\n");
        return handle_expr_arithmop_expr(quad::iopcode::div, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_MOD_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr %% expr\n");
        return handle_expr_arithmop_expr(quad::iopcode::mod, leftOperand, rightOperand, sym_table, lineno);
    }

	/* relop */

	expr* Manage_expr__expr_GT_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr > expr\n");
        return handle_expr_relop_expr(quad::iopcode::if_greater, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_GE_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr >= expr\n");
        return handle_expr_relop_expr(quad::iopcode::if_greatereq, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_LT_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr < expr\n");
        return handle_expr_relop_expr(quad::iopcode::if_less, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_LE_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr <= expr\n");
        return handle_expr_relop_expr(quad::iopcode::if_lesseq, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_EQ_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr == expr\n");
        return handle_expr_relop_expr(quad::iopcode::if_eq, leftOperand, rightOperand, sym_table, lineno);
    }
    expr* Manage_expr__expr_NE_expr(symbol_table &sym_table, unsigned int lineno, expr *leftOperand, expr *rightOperand) {
        fprintf(yyout, "expr -> expr != expr\n");
        return handle_expr_relop_expr(quad::iopcode::if_noteq, leftOperand, rightOperand, sym_table, lineno);
    }

    expr* Manage_expr__expr_AND_expr() {
        fprintf(yyout, "expr -> expr AND expr\n");
        return void_value;
    }
    expr* Manage_expr__expr_OR_expr() {
        fprintf(yyout, "expr -> expr OR expr\n");
        return void_value;
    }
    expr* Manage_expr__term() {
        fprintf(yyout, "expr -> term\n");
        return void_value;
    }

	void_t Manage_term__LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(){
		fprintf(yyout, "term -> (expr)\n");
        return void_value;
	}
	void_t Manage_term__MINUS_expr(){
		fprintf(yyout, "term -> -expr\n");
        return void_value;
	}
	void_t Manage_term__NOT_expr(){
		fprintf(yyout, "term -> NOT expr\n");
        return void_value;
	}
	void_t Manage_term__PLUS_PLUS_lvalue(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno){
		fprintf(yyout, "term -> ++lvalue\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
	}
	void_t Manage_term__lvalue_PLUS_PLUS(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno){
		fprintf(yyout, "term ->lvalue ++\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
	}
	void_t Manage_term__MINUS_MINUS_lvalue(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno){
		fprintf(yyout, "term -> --lvalue\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
	}
	void_t Manage_term__lvalue_MINUS_MINUS(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno){
		fprintf(yyout, "term -> lvalue--\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
	}
	void_t Manage_term__primary(){
		fprintf(yyout, "term -> primary\n");
        return void_value;
	}
	
    void_t Manage_assignexpr__lvalue_ASSIGN_expr(symbol_table::entry::lvalue_type lvalueType, unsigned int lineno) {
        fprintf(yyout, "assignexpr -> lvalue = expr\n");
        if(lvalueType == symbol_table::entry::lvalue_type::FUNC)
            throw syntax_error("Function id cannot be used as an l-value", lineno);
        return void_value;
    }


    expr* Manage_primary__lvalue(expr* lvalue) {
        fprintf(yyout, "primary -> lvalue\n");
		return emit_iftableitem(lvalue);
    }
    void_t Manage_primary__call() {
        fprintf(yyout, "primary -> call\n");
        return void_value;
    }
    void_t Manage_primary__objectdef() {
        fprintf(yyout, "primary -> objectdef\n");
        return void_value;
    }
    void_t Manage_primary__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS() {
        fprintf(yyout, "primary -> (funcdef)\n");
        return void_value;
    }
    void_t Manage_primary__const() {
        fprintf(yyout, "primary -> const\n");
        return void_value;
    }

    expr* Manage_lvalue__IDENTIFIER(symbol_table &sym_table, const string &identifier, unsigned int lineno) {
        fprintf(yyout, "lvalue -> IDENTIFIER\n");

        unsigned int scope = scp_handler.get_current_scope();
        unsigned int active_function_scope = scp_handler.get_active_function_scope();
        vector<symbol_table::entry*> all_entries = sym_table.recursive_lookup(identifier, scope);
        symbol_table::entry *id_entry;

        if (all_entries.empty()) { /* This identifier was not found in any enclosing scope. ==> Insert new symbol */
            id_entry = new symbol_table::var_entry(
                    scope, lineno, identifier, symbol_table::entry::LOCAL, scp_handler.get_current_ss(), scp_handler.fetch_and_incr_cur_ssoffset()
            );
            sym_table.insert(id_entry);
        } else {
            /* We need to check if there is an accessible reference in all_entries */
            try {
                id_entry = sym_table.exists_accessible_symbol(identifier, scope, active_function_scope);
            } catch(runtime_error &err) {
                throw syntax_error("Variable \'" + identifier + "\': " + err.what(), lineno);
            }
        }
        return expr::make_lvalue_expr(id_entry);
    }
    expr* Manage_lvalue__LOCAL_IDENTIFIER(symbol_table &sym_table, const string &identifier, unsigned int lineno) {
        fprintf(yyout, "lvalue -> local IDENTIFIER\n");

        unsigned int scope = scp_handler.get_current_scope();
        vector<symbol_table::entry*> scope_entries = sym_table.lookup(identifier, scope);
        vector<symbol_table::entry*> global_entries = sym_table.lookup(identifier, scope_handler::GLOBAL_SCOPE);
        symbol_table::entry *id_entry;

        if(scope_entries.empty()) {
            //Check if it shadows a library function
            bool lib_func_clash = false;
            for(const auto &global_entry: global_entries)
                if(global_entry->get_sym_type() == symbol_table::entry::LIB_FUNC)
                    lib_func_clash = true;
            if(lib_func_clash)
                throw syntax_error("Local variable \'" + identifier + "\' name shadows library function", lineno);

            //Identifier is okay. Add it to the symbol table
            id_entry = new symbol_table::var_entry(
                    scope, lineno, identifier, symbol_table::entry::LOCAL, scp_handler.get_current_ss(), scp_handler.fetch_and_incr_cur_ssoffset()
            );
            sym_table.insert(id_entry);
        } else {
            assert(scope_entries.size() == 1);
            id_entry = scope_entries.at(0);
        }
        return expr::make_lvalue_expr(id_entry);
    }
    expr* Manage_lvalue__DOUBLE_COLON_IDENTIFIER(const symbol_table &sym_table, const string &identifier, unsigned int lineno) {
        fprintf(yyout, "lvalue -> ::IDENTIFIER\n");

        /* This rule NEVER inserts to the symbol table */
        symbol_table::entry *id_entry;
        vector<symbol_table::entry*> global_entries = sym_table.lookup(identifier, scope_handler::GLOBAL_SCOPE);
        if(global_entries.empty())
            throw syntax_error("No global symbol \'" + identifier + "\' found.", lineno);

        assert(global_entries.size() == 1);
        id_entry = global_entries.at(0);
        return expr::make_lvalue_expr(id_entry);
    }
    symbol_table::entry::lvalue_type Manage_lvalue__member() {
        fprintf(yyout, "lvalue -> member\n");
        return symbol_table::entry::lvalue_type::VAR;
    }


    expr* Manage_member__lvalue_DOT_IDENTIFIER(expr* lvalue, const string &id) {
        fprintf(yyout, "member -> .IDENTIFIER\n");
        lvalue = emit_iftableitem(lvalue); /* Emit code if it is a table item. */
		expr* item = newexpr(tableitem_e); /*Make a new expression. */
		item->sym_entry = lvalue->sym_entry;
		item->index = newexpr_conststring(id); /* Const string index. */
		return item;
    }
    expr* Manage_member__lvalue_LEFT_BRACKET_expr_RIGHT_BRACKET(expr* lvalue, expr* expr) {
        fprintf(yyout, "member -> lvalue[expr]\n");
        lvalue = emit_iftableitem(lvalue);
		tableitem = newexpr(tableitem_e);
		tableitem->sym_entry = lvalue->sym_entry;
		tableitem_e->index = expr; /* The index is the expression. */		
		return tableitem;
    }
    void_t Manage_member__call_DOT_IDENTIFIER() {
        fprintf(yyout, "member -> call.IDENTIFIER\n");
        return void_value;
    }
    void_t Manage_member__call_LEFT_BRACKET_expr_RIGHT_BRAKET() {
        fprintf(yyout, "member -> [expr]\n");
        return void_value;
    }

	/* Manage_call() */
	expr* Manage_call__call_normcall(symbol_table &sym_table, unsigned int lineno, expr *call, norm_call *norm_call) {
		fprintf(yyout, " call -> call normcall\n");
		return handle_call_rule(call, norm_call->get_elist(), sym_table, lineno);
	}
	expr* Manage_call__lvalue_callsuffix(symbol_table &sym_table, unsigned int lineno, expr *lvalue, call_suffix *call_suffix){
		fprintf(yyout, "call -> lvalue callsuffix\n");

        if(call_suffix->get_type() == call_suffix::type::METHOD_CALL) {
            method_call *method_call;
            if(!(method_call = dynamic_cast<method_call*>(call_suffix)))    /*TODO: validate that this works*/
                    assert(false);  //Casting should ALWAYS be possible

            expr *self = lvalue;
            lvalue = emit_iftableitem(member_item(self, method_call->get_name()));
            call_suffix->get_elist().push_front(self);
        }
		return handle_call_rule(lvalue, call_suffix->get_elist(), sym_table, lineno);
	}
	expr* Manage_call__LEFT_PARENTHESIS_funcdef_RIGHT_PARENTHESIS_normcall(symbol_table &sym_table, unsigned int lineno,
                                                                           symbol_table::func_entry *funcdef, norm_call *norm_call) {
		fprintf(yyout, "call -> (funcdef) (elist)\n");
		expr *func = expr::make_expr(expr::type::PROGRAM_FUNC_E);
		func->sym_entry = funcdef;
		return handle_call_rule(func, norm_call->get_elist(), sym_table, lineno);
	}

	/* Manage_callsuffix() */

    call_suffix* Manage_callsuffix__normcall(norm_call* norm_call){
		fprintf(yyout, "callsuffix -> normcall\n");
		return norm_call;
	}
    call_suffix* Manage_callsuffix__methodcall(method_call* method_call){
		fprintf(yyout, "callsuffix -> methodcall\n");
		return method_call;
	}

	/* Manage_normcall() */
	norm_call* Manage_normcall__LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(deque<expr*> const &elist){
		fprintf(yyout, "normcall -> (elist)\n");
		return new norm_call(elist);
	}

	/* Manage_methodcall() */
    method_call* Manage_methodcall__DOUBLE_DOT_IDENTIFIER_LEFT_PARENTHESIS_elist_RIGHT_PARENTHESIS(const string &id, deque<expr*> const &elist) {
		fprintf(yyout, "methodcall -> ..id(elist)\n");
		return new method_call(id, elist);
	}

	/* Manage_elist() */
    deque<expr*> Manage_tmp_elist__tmp_elist_COMMA_expr(deque<expr*> const &tmp_elist, expr *expr) {
		fprintf(yyout, "tmp_elist -> tmp_elist , expr\n");
		deque<expr*> ret_val = tmp_elist;
		ret_val.push_back(expr);

		return ret_val;
	}
    deque<expr*> Manage_tmp_elist__empty(){
		fprintf(yyout, "tmp_elist -> <empty>\n");
		return deque<expr*>();  //empty double ended queue
	}
    deque<expr*> Manage_elist__empty(){
		fprintf(yyout, "elist -> <empty>\n");
		return deque<expr*>();  //empty double ended queue
	}
    deque<expr*> Manage_elist__expr_tmp_elist(expr *expr, deque<expr*> const &tmp_elist) {
        fprintf(yyout, "elist -> expr tmp_elist\n");
        deque<expr*> ret_val = tmp_elist;
        ret_val.push_front(expr);

        return ret_val;
    }

	/* Manage_objectdef()*/
	void_t Manage_objectdef_LEFT_BRACKET_elist_RIGHT_BRACKET(){
		fprintf(yyout, "objectdef -> [ elist ]\n");
		return void_value;
	}
	void_t Manage_objectdef_LEFT_BRACKET_indexed_RIGHT_BRACKET(){
		fprintf(yyout, "objectdef -> [ indexed ]\n");
		return void_value;
	}

	/* Manage_indexed */
	void_t Manage_tmp_indexed_tmp_indexed_COMMA_indexedelem(){
		fprintf(yyout, "indexed -> indexedelem,...,indexedelem\n");
		return void_value;
	}
	void_t Manage_tmp_indexed_empty(){
		fprintf(yyout, "indexed -> indexedelem\n");
		return void_value;
	}
    void_t Manage_indexed__indexedelem_tmp_indexed() {
        fprintf(yyout, "indexed -> indexelem tmp_indexed\n");
        return void_value;
    }

	/* Manage_indexedelem() */
	void_t Manage_indexedelem_LEFT_BRACE_expr_COLON_expr_RIGHT_BRACE(){
		fprintf(yyout, "indexedelem -> { expr : expr }\n");
		return void_value;
	}

	/* Manage_block() */
    void_t Manage_block_open__LEFT_BRACE() {
        fprintf(yyout, "block_open -> {\n");
        scp_handler.increase_scope();
        return void_value;
    }
    void_t Manage_block_close__RIGHT_BRACE() {
        fprintf(yyout, "block_close -> }\n");
        scp_handler.decrease_scope();
        return void_value;
    }
	void_t Manage_block__block_open_stmts_block_close(symbol_table &sym_table) {
		sym_table.hide(scp_handler.get_current_scope()+1);    //+1 because the block closed and thus we exited that scope
		fprintf(yyout, "block -> { stmts }\n");
		return void_value;
	}

	/* Manage_funcdef() */
    string Manage_funcname__IDENTIFIER(const string &id) {
        /* Lookup operation is done by Manage_funcdef__FUNCTION_tmp_funcdef */
        fprintf(yyout, "funcname -> IDENTIFIER\n");
        return id;
    }
    string Manage_funcname__empty() {
        fprintf(yyout, "funcname -> <empty>\n");
        return string();    /* Empty string */
    }

    symbol_table::func_entry* Manage_funcprefix__FUNCTION_funcname(symbol_table &sym_table, const std::string &id, unsigned int lineno) {
        fprintf(yyout, "funcprefix -> FUNCTION funcname\n");

        unsigned int scope = scp_handler.get_current_scope();
        vector<symbol_table::entry*> cur_scope_entries = sym_table.lookup(id, scope);  /* Look up only at the current scope */
        vector<symbol_table::entry*> global_entries = sym_table.lookup(id, scope_handler::GLOBAL_SCOPE);  /* Look up only at the global scope */

        //Sanity checks
        if(!cur_scope_entries.empty())
            throw syntax_error("Function definition \'" + id
                               + "\' name clashes with already defined function or variable name in the same scope", lineno);

        for(const auto &entry : global_entries)
            if(entry->get_sym_type() == symbol_table::entry::LIB_FUNC)
                throw syntax_error("Function definition \'" + id + "\' name shadows library function", lineno);

		//Insert in the symbol table
		symbol_table::func_entry *new_func_entry = new symbol_table::func_entry(
			scope, lineno, id, symbol_table::entry::USER_FUNC, icode_gen.next_quad_label()
		);
		sym_table.insert(new_func_entry);
		icode_gen.emit_quad(new quad(quad::iopcode::funcstart, expr::make_lvalue_expr(new_func_entry), nullptr, nullptr, lineno));

        scp_handler.enter_formal_arg_ss();
        scp_handler.increase_scope();
		return new_func_entry;
	}
    void_t Manage_funcargs__LEFT_PARENTHESIS_idlist_RIGHT_PARENTHESIS() {
        fprintf(yyout, "funcars -> ( idlist )\n");
        scp_handler.exit_formal_arg_ss();
        scp_handler.decrease_scope();

        scp_handler.enter_function_ss();
        //"block" grammar rule will increase scope again
        return void_value;
    };

    unsigned int Manage_funcbody__block() {
        fprintf(yyout, "funcbody -> block\n");
        return scp_handler.exit_function_ss();    //scope already reduced by "block" grammar rule
    }
    symbol_table::func_entry* Manage_funcdef__funcprefix_funcargs_funcbody(symbol_table::func_entry *func_entry,
                                                                           unsigned int lineno, unsigned int total_func_locals) {
        fprintf(yyout, "funcdef -> funcprefix funcargs funcbody\n");
        func_entry->set_total_locals(total_func_locals);

		icode_gen.emit_quad(new quad(quad::iopcode::funcend, expr::make_lvalue_expr(func_entry), nullptr, nullptr, lineno));
        return func_entry;
    }

	/* Manage_const() */
	void_t Manage_const_CONST_INT(){
		fprintf(yyout, "const -> CONST_INT\n");
		return void_value;
	}
	void_t Manage_const_CONST_REAL(){
		fprintf(yyout, "const -> CONST_REAL\n");
		return void_value;
	}
	void_t Manage_const_CONST_STR(){
		fprintf(yyout, "const -> CONST_STRING\n");
		return void_value;
	}
	void_t Manage_const_NIL(){
		fprintf(yyout, "const -> nil\n");
		return void_value;
	}
	void_t Manage_const_BOOL_TRUE(){
		fprintf(yyout, "const -> true\n");
		return void_value;
	}
	void_t Manage_const_BOOL_FALSE(){
		fprintf(yyout, "const -> false\n");
		return void_value;
	}

	/* Manage_idlist() */
    vector<string> Manage_tmp_idlist__empty() {
        fprintf(yyout, "tmp_idlist -> <empty>\n");
        return vector<string>();
    }
    vector<string> Manage_tmp_idlist__tmp_idlist_COMMA_IDENTIFIER(vector<string> &tmp_id_list, const string &identifier) {
        /* Gather the IDs. We postpone the processing until we have gathered all the IDs from the idlsit */
        tmp_id_list.push_back(identifier);
		fprintf(yyout, "tmp_idlist -> tmp_idlist , IDENTIFIER\n");
        return tmp_id_list;
	}
    vector<string> Manage_idlist__IDENTIFIER_tmp_idlist(symbol_table &sym_table, vector<string> tmp_id_list, string identifier, unsigned int lineno) {
        //The grammar rule idlist-> (zero, one, or more "id") is used only by the grammar rule funcdef

        fprintf(yyout, "idlist -> IDENTIFIER tmp_idlist\n");
        unsigned int scope = scp_handler.get_current_scope();

        tmp_id_list.push_back(identifier);
        for(const auto &cur_id : tmp_id_list) {
            vector<symbol_table::entry*> cur_scope_entries = sym_table.lookup(cur_id, scope);  /* Look up only at the current scope */
            vector<symbol_table::entry*> global_entries = sym_table.lookup(cur_id, scope_handler::GLOBAL_SCOPE);  /* Look up only at the global scope */

            if(cur_scope_entries.empty()) {
                sym_table.insert(new symbol_table::var_entry(
                    scope, lineno, cur_id, symbol_table::entry::FORMAL_ARG, scp_handler.get_current_ss(), scp_handler.fetch_and_incr_cur_ssoffset()
                ));
            } else {
                throw syntax_error("Formal Argument \'" + cur_id + "\' defined multiple times", lineno);
            }

            for(const auto &entry : global_entries)
                if(entry->get_sym_type() == symbol_table::entry::LIB_FUNC)
                    throw syntax_error("Formal Argument \'" + cur_id + "\' shadows library function", lineno);
        }
        return tmp_id_list;
    }
    vector<string> Manage_idlist__empty(){
		fprintf(yyout, "idlist -> <empty>\n");
		return vector<string>();
	}

	/* Manage_ifstmt() */
	unsigned int Manage_IF_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(expr* expr, unsigned int lineno){
		icode_gen.emit_quad(new quad(iopcode::if_eq, expr, newexpr_constbool(true), nullptr, lineno) ,icode_gen.next_quad_label() + 2);
		icode_gen.emit_quad(new quad(iopcode::jump, nullptr, nullptr, nullptr, lineno),0);
		return icode_gen.next_quad_label();
	};
	unsigned int Manage_elseprefix_ELSE(unsigned int lineno){
		icode_gen.emit_quad(new quad(iopcode::jump, nullptr, nullptr, nullptr, lineno),0);
		return icode_gen.next_quad_label();
	}
	void_t  Manage_ifprefix_stmt(unsigned int ifprefix){
		fprintf(yyout, "ifstmt -> if ( expr ) stmt\n");
		icode_gen.patch_label(ifprefix, icode_gen.next_quad_label());
		return void_value;
	}
	void_t Manage_ifprefix_stmt_elseprefix_stmt(unsigned int ifprefix , unsigned int elseprefix){
		fprintf(yyout, "ifstmt -> if ( expr ) stmt else stmt\n");
		icode_gen.patch_label(ifprefix, elseprefix + 1);
		icode_gen.patch_label(elseprefix, icode_gen.next_quad_label());
		return void_value;
	}

	/* Manage_whilestmt() */
	unsigned int Manage_whilestart_WHILE(){
		return icode_gen.next_quad_label();
	}
	unsigned int Manage_whilecond_LEFT_PARENTHESIS_expr_RIGHT_PARENTHESIS(expr * expr){
		icode_gen.emit_quad(new quad(iopcode::if_eq, expr, newexpr_constbool(true), nullptr, lineno), icode_gen.next_quad_label() + 2);
		icode_gen.emit_quad(new quad(iopcode::jump, nullptr, nullptr, nullptr, lineno), 0);
		return icode_gen.next_quad_label();
	}
	void_t Manage_whilestmt_whilestart_whilecond_stmt(unsigned int whilestart , unsigned int whilecond){
		fprintf(yyout, "whilestmt -> while ( expr ) stmt\n");
		icode_gen.emit_quad(new quad(iopcode::jump, whilestart, nullptr, nullptr, lineno), 0);
		icode_gen.patch_label(whilecond, icode_gen.next_quad_label());
		/*TODO:*/
		/*icode_gen.patch_label("$stmt.breaklist", icode_gen.next_quad_label());
		icode_gen.patch_label("$stmt.contlist", icode_gen.next_quad_label());*/
		return void_value;
	}

	/* Manage_forstmt() */
	void_t Manage_FOR_LEFT_PARENTHESIS_elist_SEMICLON_expr_SEMICOLON_elist_RIGHT_PARENTHESIS_stmt(){
		fprintf(yyout, "forstmt -> for (elist;expr;elist) stmt\n");
		return void_value;
	}

	/* Manage_returnstmt() */
	void_t Manage_RETURN_SEMICOLON(){
		fprintf(yyout, "returnstmt -> return ;\n");
		return void_value;
	}
	void_t Manage_RETURN_expr_SEMICOLON(){
		fprintf(yyout, "returnstmt -> return expr ;\n");
		return void_value;
	}
}