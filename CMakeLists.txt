cmake_minimum_required(VERSION 3.8)
project(hy340_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_BUILD_TYPE DEBUG)

add_executable(acc
        alpha_lex/alpha_flex.cpp alpha_lex/alpha_flex.h

        common_interface/errors/numeric_error.cpp common_interface/errors/numeric_error.h

        common_interface/arithm_ops.cpp common_interface/arithm_ops.h
        common_interface/Builtin_funcs.h common_interface/Builtin_funcs.cpp
        common_interface/vm_types.cpp common_interface/vm_types.h
        common_interface/Constants_pool.h common_interface/Constants_pool.cpp
        common_interface/Constants.h

        intermediate_code/call_suffix.cpp intermediate_code/call_suffix.h
        intermediate_code/expr.cpp intermediate_code/expr.h
        intermediate_code/for_prefix.cpp intermediate_code/for_prefix.h
        intermediate_code/icode_generator.cpp intermediate_code/icode_generator.h
        intermediate_code/quad.cpp intermediate_code/quad.h
        intermediate_code/semantic_error.cpp intermediate_code/semantic_error.h
        intermediate_code/types.h

        syntax_analyzer/alpha_bison.cpp syntax_analyzer/alpha_bison.h
        syntax_analyzer/hidden_var_handler.cpp syntax_analyzer/hidden_var_handler.h
        syntax_analyzer/loop_handler.cpp syntax_analyzer/loop_handler.h
        syntax_analyzer/parser_manager.cpp syntax_analyzer/parser_manager.h
        syntax_analyzer/scope_handler.cpp syntax_analyzer/scope_handler.h
        syntax_analyzer/symbol_table.cpp syntax_analyzer/symbol_table.h
        syntax_analyzer/syntax_error.cpp syntax_analyzer/syntax_error.h
        syntax_analyzer/types.h

        target_code/VMcode_generator.cpp target_code/VMcode_generator.h
        target_code/types.h target_code/types.cpp

        target_code/generate_funcs/helpers.cpp
        target_code/generate_funcs/arithmetic.cpp
        target_code/generate_funcs/tables.cpp
        target_code/generate_funcs/misc.cpp
        target_code/generate_funcs/relational.cpp
        target_code/generate_funcs/functions.cpp

        main.cpp
        not_implemented_error.cpp not_implemented_error.h
        utilities.cpp utilities.h target_code/Binary_writer.cpp target_code/Binary_writer.h)

add_executable(avm
        not_implemented_error.cpp not_implemented_error.h

        common_interface/errors/numeric_error.cpp common_interface/errors/numeric_error.h

        common_interface/Builtin_funcs.h common_interface/Builtin_funcs.cpp
        common_interface/arithm_ops.cpp common_interface/arithm_ops.h
        common_interface/vm_types.cpp common_interface/vm_types.h
        common_interface/Constants_pool.cpp common_interface/Constants_pool.h
        common_interface/Constants.h

        virtual_machine/AVM/dispatcher/arithmetic.cpp
        virtual_machine/AVM/dispatcher/functions.cpp
        virtual_machine/AVM/dispatcher/misc.cpp
        virtual_machine/AVM/dispatcher/relational.cpp
        virtual_machine/AVM/dispatcher/tables.cpp

        virtual_machine/AVM/builtin_funcs/misc.cpp
        virtual_machine/AVM/builtin_funcs/math.cpp
        virtual_machine/AVM/builtin_funcs/tables.cpp
        virtual_machine/AVM/builtin_funcs/functions.cpp

        virtual_machine/AVM/Program_stack/Program_stack.cpp virtual_machine/AVM/Program_stack/Program_stack.h
        virtual_machine/AVM/Program_stack/Stack_manipulator.cpp virtual_machine/AVM/Program_stack/Stack_manipulator.h

        virtual_machine/AVM/errors/alpha_runtime_error.cpp virtual_machine/AVM/errors/alpha_runtime_error.h
        virtual_machine/AVM/errors/internal_error.cpp virtual_machine/AVM/errors/internal_error.h

        virtual_machine/AVM/AVM.cpp virtual_machine/AVM/AVM.h
        virtual_machine/AVM/Memcell.cpp virtual_machine/AVM/Memcell.h
        virtual_machine/AVM/Table.cpp virtual_machine/AVM/Table.h

        virtual_machine/RDP/RDP.cpp virtual_machine/RDP/RDP.h
        virtual_machine/RDP/Token.cpp virtual_machine/RDP/Token.h
        virtual_machine/RDP/Lookahead.cpp

        common_interface/utilities.h
        virtual_machine/main.cpp)