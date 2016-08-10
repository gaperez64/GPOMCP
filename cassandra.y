%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
%defines
%define parser_class_name {CassParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
#include <string>
#include <vector>
class CassDriver;
enum ElemRefType { ELEMREFTYPE_ID, ELEMREFTYPE_NAME, ELEMREFTYPE_ALL };

struct ElemRef {
    ElemRefType type;
    std::string name;
    int id;
};
}

%param {CassDriver &driver}
%locations
%initial-action
{
    @$.begin.filename = @$.end.filename = &driver.file;
};
%define parse.trace
%define parse.error verbose

%code
{
#include <iterator>
#include <iostream>
#include <algorithm>
#include "cassandra-driver.h"
}

%define api.token.prefix {TOK_}
%token
END 0            "end of file"
DISCOUNT         "discount"
VALUES           "values"
STATES           "states"
ACTIONS          "actions"
OBSERVATIONS     "observations"
T                "T"
O                "O"
R                "R"
UNIFORM          "uniform"
IDENTITY         "identity"
REWARD           "reward"
COST             "cost"
START            "start"
INCLUDE          "include"
EXCLUDE          "exclude"
RESET            "reset"
COLON            ":"
ASTERISK         "*"
PLUS             "+"
MINUS            "-"
;
%token <float> FLOAT "float"
%token <int> INT "int"
%token <std::string> STRING "string"
%type <int> optional_sign value_tail
%type <float> prob number
%type <std::vector<std::string>> ident_list state_tail action_tail
%type <std::vector<std::string>> obs_param_tail
%type <ElemRef> state paction obs

%printer {
    if (!$$.empty()) {
        std::copy($$.begin(), $$.end() - 1,
                  std::ostream_iterator<std::string>(yyoutput, ","));
        yyoutput << $$.back();
    }
} <std::vector<std::string>>;
%printer { yyoutput << $$.name; } <ElemRef>;
%printer { yyoutput << $$; } <*>;

%%
%start pomdp_file;

pomdp_file: preamble start_state param_list
;

preamble: preamble param_type 
| /* empty */
;

param_type: discount_param
| value_param
| state_param
| action_param
|
obs_param
;

discount_param: DISCOUNT COLON number { driver.setDiscount($3); }
;

value_param: VALUES COLON value_tail { driver.setWeightSign($3); }
;

value_tail: REWARD { $$ = 1; }
| COST { $$ = -1; }
;

state_param: STATES COLON state_tail { driver.setStates($3); }
;

state_tail: INT { $$.resize($1); }
| ident_list { std::swap($$, $1); }
;

action_param: ACTIONS COLON action_tail { driver.setActions($3); }
;

action_tail: INT { $$.resize($1); }
| ident_list { std::swap($$, $1); }
;

obs_param: OBSERVATIONS COLON obs_param_tail { driver.setObservations($3); }
;

obs_param_tail: INT { $$.resize($1); }
| ident_list { std::swap($$, $1); }
;

start_state:  START COLON u_matrix
| START COLON STRING
| START INCLUDE COLON start_state_list
| START EXCLUDE COLON start_state_list
| /* empty */
;

start_state_list: start_state_list state
| state
;

param_list: param_list param_spec
| /* empty */
;

param_spec: trans_prob_spec
| obs_prob_spec 
| reward_spec
;

trans_prob_spec: T COLON trans_spec_tail
;

trans_spec_tail: paction COLON state COLON state prob {
    driver.addTransition($3, $1, $5, $6);
}
| paction COLON state u_matrix  { assert(false); }
| paction ui_matrix { assert(false); }
;

obs_prob_spec: O COLON obs_spec_tail
;

obs_spec_tail: paction COLON state COLON obs prob {
    driver.addObsTransition($3, $1, $5, $6);
}
| paction COLON state u_matrix { assert(false); }
| paction u_matrix { assert(false); }
;

reward_spec: R COLON  reward_spec_tail
;

reward_spec_tail: paction COLON state COLON state COLON obs number {
    driver.addWeight($3, $1, $5, $7, $8);
}
| paction COLON state COLON state num_matrix { assert(false); }
| paction COLON state num_matrix { assert(false); }
;

ui_matrix: UNIFORM 
| IDENTITY 
| prob_matrix
;

u_matrix: UNIFORM 
| RESET
| prob_matrix
;

prob_matrix: prob_matrix prob
| prob
;

num_matrix: num_matrix number
| number
;

state: INT { $$.type = ELEMREFTYPE_ID; $$.id = $1; }
| STRING { $$.type = ELEMREFTYPE_NAME; std::swap($$.name, $1); } 
| ASTERISK { $$.type = ELEMREFTYPE_ALL; $$.name = "*"; }
;

paction: INT { $$.type = ELEMREFTYPE_ID; $$.id = $1; }
| STRING { $$.type = ELEMREFTYPE_NAME; std::swap($$.name, $1); } 
| ASTERISK { $$.type = ELEMREFTYPE_ALL; $$.name = "*"; }
;

obs: INT { $$.type = ELEMREFTYPE_ID; $$.id = $1; }
| STRING { $$.type = ELEMREFTYPE_NAME; std::swap($$.name, $1); } 
| ASTERISK { $$.type = ELEMREFTYPE_ALL; $$.name = "*"; }
;

ident_list: ident_list STRING { std::swap($$, $1); $$.push_back($2); }
| STRING { $$.push_back($1); }
;

prob: INT { $$ = $1; }
| FLOAT { $$ = $1; }
;

number: optional_sign INT { $$ = $1 * $2; }
| optional_sign FLOAT { $$ = $1 * $2; }
;

optional_sign: PLUS { $$ = 1; }
| MINUS { $$ = -1; }
| /* empty */ { $$ = 1; }
;
%%

void yy::CassParser::error(const location_type &l, const std::string &m) {
    driver.error(l, m);
}
