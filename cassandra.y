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
class CassDriver;
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

discount_param: DISCOUNT COLON number
;

value_param: VALUES COLON value_tail
;

value_tail: REWARD
| COST
;

state_param: STATES COLON state_tail
;

state_tail: INT
| ident_list
;

action_param: ACTIONS COLON action_tail
;

action_tail: INT
| ident_list
;

obs_param: OBSERVATIONS COLON obs_param_tail
;

obs_param_tail: INT
| ident_list
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

trans_spec_tail: paction COLON state COLON state prob
| paction COLON state u_matrix 
|  paction ui_matrix
;

obs_prob_spec: O COLON obs_spec_tail
;

obs_spec_tail: paction COLON state COLON obs prob
| paction COLON state u_matrix
| paction u_matrix
;

reward_spec: R COLON  reward_spec_tail
;

reward_spec_tail: paction COLON state COLON state COLON obs number 
| paction COLON state COLON state num_matrix
| paction COLON state num_matrix
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

state: INT
| STRING
| ASTERISK
;

paction: INT
| STRING
| ASTERISK
;

obs: INT
| STRING
| ASTERISK
;

ident_list: ident_list STRING
| STRING
;

prob: INT
| FLOAT
;

number: optional_sign INT
| optional_sign FLOAT
;

optional_sign: PLUS
| MINUS
| /* empty */
;
%%

void yy::CassParser::error(const location_type &l, const std::string &m) {
    driver.error(l, m);
}
