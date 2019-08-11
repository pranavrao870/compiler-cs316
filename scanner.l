%{
#include <stdlib.h>
#include "program.hh"
#include "procedure.hh"
#include "symbol-table.hh"
#include "ast.hh"
#include "parser.tab.h"
int lineno = 1;
%}

digit [0-9]
operator [-+*/]
alpha [a-zA-Z_]
meta [,;\(\)\{\}\?:]
spaces [(\t|" ")*]

%%

"//".* ;
"return" {store_token_name("RETURN",yytext,yylineno); return RETURN;}
"main"   {store_token_name("MAIN", yytext, yylineno); return MAIN;}
"if" 	{store_token_name("IF",yytext,yylineno); return IF;}
"else" {store_token_name("ELSE",yytext,yylineno); return ELSE;}
"do" 	{store_token_name("DO",yytext,yylineno); return DO;}
"while" {store_token_name("WHILE",yytext,yylineno); return WHILE;}
"int" {store_token_name("INTEGER",yytext,yylineno); return INTEGER;}
"float" {store_token_name("FLOAT",yytext,yylineno); return FLOAT;}
"void" {store_token_name("VOID",yytext,yylineno);  return VOID;}
"print" {store_token_name("PRINT", yytext, yylineno); return PRINT;}
"<="	{store_token_name("LESS_THAN_EQUAL",yytext,yylineno); return LTE;}
">="	{store_token_name("GREATER_THAN_EQUAL",yytext,yylineno); return GTE;}
"=="	{store_token_name("EQUAL",yytext,yylineno); return E;}
"<"	{store_token_name("LESS_THAN",yytext,yylineno); return LT;}
">"	{store_token_name("GREATER_THAN",yytext,yylineno); return GT;}
"!="	{store_token_name("NOT_EQUAL",yytext,yylineno); return NE;}
"&&"	{store_token_name("AND",yytext,yylineno); return AND;}
"||"	{store_token_name("OR",yytext,yylineno); return OR;}
"!"	{store_token_name("NOT",yytext,yylineno); return NOT;}
{operator} {store_token_name("ARITHOP",yytext,yylineno); yylval.string_value = new string(yytext); return yytext[0];}
{meta} {store_token_name("META CHAR",yytext,yylineno); yylval.string_value = new string(yytext); return yytext[0];}
{digit}+\.{digit}* {store_token_name("FNUM",yytext,yylineno); yylval.double_value = atof(yytext) ; return DOUBLE_NUMBER;}
{digit}*\.{digit}+ {store_token_name("FNUM",yytext,yylineno); yylval.double_value = atof(yytext) ; return DOUBLE_NUMBER;}
{digit}+ {store_token_name("NUM",yytext,yylineno); yylval.integer_value = atoi(yytext) ;return INTEGER_NUMBER;}
= {store_token_name("ASSIGN_OP",yytext,yylineno); yylval.string_value = new string(yytext); return ASSIGN;}
{alpha}({alpha}|{digit})* {store_token_name("NAME",yytext,yylineno); yylval.string_value = new string(yytext); return NAME;}
\n {lineno++;}
{spaces} ;
. {printf("Error: Unmatched character\n"); exit(1);}
%%