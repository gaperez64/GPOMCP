%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include "cassandra-driver.h"
#include "cassandra-parser.h"
// work around an incompatibility in flex
#undef yywrap
#define yywrap() 1
// the location of the token
static yy::location loc;
%}

%option noyywrap nounput batch debug noinput

%{
    // Code ran every time a pattern is matched
    #define YY_USER_ACTION loc.columns(yyleng);
%}

%%

%{
    // Code called every time yylex is called
    loc.step();
%}

#[^$]*$          loc.step();                    /* ignore comments    */
[\n]+            loc.lines(yyleng); loc.step(); /* ignore end of line */
[ \t]+           loc.step();                    /* ignore whitespace  */

discount         return yy::CassParser::make_DISCOUNT(loc);
values           return yy::CassParser::make_VALUES(loc);
states           return yy::CassParser::make_STATES(loc);
actions          return yy::CassParser::make_ACTIONS(loc);
observations     return yy::CassParser::make_OBSERVATIONS(loc);
T                return yy::CassParser::make_T(loc);
O                return yy::CassParser::make_O(loc);
R                return yy::CassParser::make_R(loc);
uniform          return yy::CassParser::make_UNIFORM(loc);
identity         return yy::CassParser::make_IDENTITY(loc);
reward           return yy::CassParser::make_REWARD(loc);
cost             return yy::CassParser::make_COST(loc);
start            return yy::CassParser::make_START(loc);
include          return yy::CassParser::make_INCLUDE(loc);
exclude          return yy::CassParser::make_EXCLUDE(loc);
reset            return yy::CassParser::make_RESET(loc);
:                return yy::CassParser::make_COLON(loc);
"*"              return yy::CassParser::make_ASTERISK(loc);
"+"              return yy::CassParser::make_PLUS(loc);
"-"              return yy::CassParser::make_MINUS(loc);
0|[1-9][0-9]*    {
    errno = 0;
    long n = strtol(yytext, NULL, 10);
    if (!(INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
        driver.error(loc, "integer is out of range");
    return yy::CassParser::make_INT(n, loc);
}
([0-9]+"."[0-9]*|"."[0-9]+|[0-9]+)([eE][-+]?[0-9]+)? {
    errno = 0;
    double n = strtod(yytext, NULL);
    return yy::CassParser::make_FLOAT(n, loc);
}
[a-zA-Z]([a-zA-Z0-9]|[\_\-])* {
    return yy::CassParser::make_STRING(yytext, loc);
}
<<EOF>>          return yy::CassParser::make_END(loc);
%%

void CassDriver::beginScan() {
    yy_flex_debug = trace_scanning;
    if (file.empty() || file == "-")
        yyin = stdin;
    else if (!(yyin = fopen(file.c_str(), "r"))) {
        error("cannot open " + file + ": " + strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void CassDriver::endScan() {
    fclose(yyin);
}
