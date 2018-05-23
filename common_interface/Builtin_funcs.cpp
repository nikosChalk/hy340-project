

#include "Builtin_funcs.h"

using namespace std;

const string Builtin_funcs::libname_print               = string("print");
const string Builtin_funcs::libname_input               = string("input");
const string Builtin_funcs::libname_objectmemberkeys    = string("objectmemberkeys");
const string Builtin_funcs::libname_objecttotalmembers  = string("objecttotalmembers");
const string Builtin_funcs::libname_objectcopy          = string("objectcopy");
const string Builtin_funcs::libname_totalarguments      = string("totalarguments");
const string Builtin_funcs::libname_argument            = string("argument");
const string Builtin_funcs::libname_typeof              = string("typeof");
const string Builtin_funcs::libname_strtonum            = string("strtonum");
const string Builtin_funcs::libname_sqrt                = string("sqrt");
const string Builtin_funcs::libname_cos                 = string("cos");
const string Builtin_funcs::libname_sin                 = string("sin");

const vector<string> Builtin_funcs::LIB_FUNC_NAMES = {
    libname_print, libname_input, libname_objectmemberkeys, libname_objecttotalmembers,
    libname_objectcopy, libname_totalarguments, libname_argument, libname_typeof,
    libname_strtonum, libname_sqrt, libname_cos, libname_sin
};