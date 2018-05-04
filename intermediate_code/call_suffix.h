/**
 * Semantic value of "callsuffix" non-terminal symbol
 */

#ifndef HY340_PROJECT_CALL_SUFFIX_H
#define HY340_PROJECT_CALL_SUFFIX_H

#include "expr.h"

namespace intermediate_code {

    class call_suffix {
    public:
        enum type {NORM_CALL, METHOD_CALL};

        /**
         * Returns the expression list of the arguments.
         * @return The expression list
         */
        std::deque<expr*>& get_elist();

        /**
         * Returns the type of the callsuffix
         * @return The type of the callsuffix
         */
        type get_type() const;

    protected:
        /**
         * Initializes the call_suffix with the given attributes
         * @param suffix_type The type of the call suffix
         * @param elist The expression list of the function's arguments, with the same order as they appear during the function call.
         * e.g.: If function call is "f(a,b)" then elist should be "a,b"
         */
        call_suffix(type suffix_type, std::deque<expr*> const &elist);

    private:
        /**
         * expression list of the arguments. If function call is "f(a,b)" and elist should be "a,b"
         */
        std::deque<expr*> elist;
        type suffix_type;
    };

    class method_call:public call_suffix {
    public:
        /**
         * Creates a new callsuffix with type METHOD_CALL and the given attributes
         * @param name The name of the method call
         * @param elist The expression list of the function's arguments, with the same order as they appear during the function call.
         * e.g.: If function call is "f(a,b)" then elist should be "a,b"
         */
        method_call(const std::string &name, std::deque<expr*> const &elist);

        /**
         * Returns the name of the method call
         * @return The name
         */
        const std::string& get_name() const;

    private:
        std::string name;
    };

    class norm_call:public call_suffix {
    public:
        /**
         * Creates a new callsuffix with type NORM_CALL and the given elist.
         * @param elist The expression list of the function's arguments, with the same order as they appear during the function call.
         * e.g.: If function call is "f(a,b)" then elist should be "a,b"
         */
        explicit norm_call(std::deque<expr*> const &elist);
    };

};

#endif //HY340_PROJECT_CALL_SUFFIX_H
