%{
#include <stdio.h>
#include <string>
#include <vector>
#include <list>
#include "program.hh"
#include "procedure.hh"
#include "symbol-table.hh"
#include "ast.hh"
using namespace std;
int yylex(void);
    ////////////////////////////////
extern "C" void yyerror(char *s){printf("cs316: Error Parsing Error\n");}
    ////////////////////////////////
extern int lineno;
Symbol_Table* global_t = new Symbol_Table();
Symbol_Table* local_t = new Symbol_Table();
Procedure *p;
string curr_name;
Data_Type curr_return_type = void_data_type;
Data_Type curr_fn_ret_type = void_data_type;

%}
%token IF
%token ELSE 
%token DO 
%token WHILE
%token NE 
%token E 
%token GTE 
%token LTE 
%token LT 
%token GT 
%token AND 
%token OR 
%token NOT 
%token RETURN
%token MAIN
%token INTEGER
%token FLOAT
%token VOID
%token PRINT
%token DOUBLE_NUMBER
%token INTEGER_NUMBER
%token ASSIGN
%token NAME

%left '?' ':'
%left OR
%left AND
%left E NE
%left GTE GT LT LTE
%left '+' '-'
%left '*' '/'
%right NOT


%union {
  int integer_value;
  double double_value;
  string* string_value;
  Program* program;
  Symbol_Table* symbol_table;
  Symbol_Table_Entry* symbol_table_entry;
  vector<Symbol_Table_Entry* > *vste;
  vector<string* > *vst;
  Data_Type data_type;
  Procedure* procedure;
  list<Ast* > *last;
  Ast* ast;
  Sequence_Ast* sast;
  Relational_Op relational_op;
  pair<string*, Data_Type> *pairsd;
}

%type <double_value> DOUBLE_NUMBER
%type <integer_value> INTEGER_NUMBER
%type <string_value> NAME
%type <program> program
%type <symbol_table> dl1
%type <vste> vd1
%type <symbol_table> dl2
%type <vste> vd2
%type <vst> vl
%type <vst> evl
%type <data_type> type
%type <procedure> pd
%type <last> sl
%type <sast> bblock
%type <sast> nsl
%type <ast> as
%type <ast> ps
%type <ast> func_call
%type <last> param_list
%type <last> eparam_list
%type <ast> ret
%type <ast> expr
%type <ast> expr0
%type <ast> expr1
%type <ast> expr2
%type <ast> expr3
%type <ast> cond
// %type <ast> cond1
// %type <ast> cond2
// %type <ast> cond3
%type <ast> temp
%type <ast> temp1
%type <ast> ans
%type <ast> ans1
%type <ast> ifstmt
%type <ast> uifstmt
%type <ast> mifstmt
%type <ast> cstmt
%type <relational_op> relop
%type<symbol_table> arg_list
%type<symbol_table> narg_list
%type<symbol_table_entry> argu
%type<pairsd> sing_dec

%%

program : dl1 epd_list
        {
          program_object.set_global_table(*($1));
          $$ = &program_object;
        }
        ;
dl1      : /* empty */ {$$ = global_t;}
        | dl1 vd1 ';' {for (int i = 0; i < ($2)->size(); i++) {
                    ($1)->push_symbol((*($2))[i]); 
                  }
                delete $2;
                $$ = $1;
                }
        ;

sing_dec : type NAME
          {
            string us("_");
            string* sss = new string(*($2) + us);
            pair<string*, Data_Type> *pa = new pair<string*, Data_Type>();
            pa->first = sss;
            pa->second = $1;
            $$ = pa;
          }

vd1      : sing_dec evl 
          {
            printf("vd1 getting reduced man\n");
            fflush(stdout);
            Data_Type ty = $1->second;

            if(global_t->variable_in_symbol_list_check(*($1->first)))
            { 
              printf("cs316: Error Multiple declarations of a variable\n");
            }
            ////////////////////////////////
            vector<Symbol_Table_Entry*> *ans = new vector<Symbol_Table_Entry*>();

            Symbol_Table_Entry * ss = new Symbol_Table_Entry(*($1->first), ty, lineno);
            ans->push_back(ss);            
             
            for(int i=0;i<($2)->size();i++){
              ////////////////////////////////
              if(global_t->variable_in_symbol_list_check(*(*($2))[i]))
              { 
                printf("cs316: Error Multiple declarations of a variable\n");
              }
              ////////////////////////////////
              Symbol_Table_Entry * s = new Symbol_Table_Entry(*(*($2))[i], ty, lineno);
              ans->push_back(s);
            }
            delete $2;
            $$ = ans;
          }
        ;
dl2      : /* empty */ {$$ = local_t;}
        | dl2 vd2 ';' { if ($2 != NULL) {for (int i = 0; i < ($2)->size(); i++) {
                    ($1)->push_symbol((*($2))[i]); 
                  }
                  delete $2;
                }
                $$ = $1;
                }
        ;
vd2      : /* empty */ {$$ = NULL;}
         | type vl  {
                        vector<Symbol_Table_Entry*> *ans = new vector<Symbol_Table_Entry*>(); 
                        for(int i=0;i<($2)->size();i++){
                          ////////////////////////////////
                          if(local_t->variable_in_symbol_list_check(*(*($2))[i]))
                          { 
                            printf("cs316: Error Multiple declarations of a variable\n");
                          }
                          ////////////////////////////////
                          Symbol_Table_Entry * s = new Symbol_Table_Entry(*(*($2))[i],$1,lineno);
                          ans->push_back(s);
                        }
                        delete $2;
                        $$ = ans;
                      }
        ;
vl      : NAME  {  string us("_"); string* sss = new string(*($1) + us) ; vector<string*> *ans = new vector<string* >(1,sss); $$ = ans;}
        | vl ',' NAME
        {
          string us("_");
          string* sss = new string(*($3) + us);
          ($1)->push_back(sss);
          $$ = $1;
        }
        ;

evl     : /* empty */ {vector<string*> *ans = new vector<string*>(); $$ = ans;}
        | vl
        ;

type    : INTEGER {$$ = int_data_type;}
        | FLOAT {$$ = double_data_type;}
        ;

epd_list : pd  { string main_str("main"); program_object.set_proc_to_map(main_str, $1); }
        | pd_list pd { string main_str("main"); program_object.set_proc_to_map(main_str, $2); }
        ;

pd_list : pd_oth
        | pd_list pd_oth
        ;

pd_oth  : sing_dec '('  narg_list ')' 
                    {
                      string *na = $1->first;
                      if(program_object.is_procedure_exists(*na)){
					            	printf("cs316: Error Current procedure already defined \n");
                        exit(1);
					            }
                      p = new Procedure($1->second, *na, lineno);
                      program_object.set_proc_to_map(*na, p);
                      p->set_formal_param_list(*$3);
                      curr_name = *na;
                      curr_fn_ret_type = $1->second;
                    }
                          '{' dl2 sl '}'
                    {
                      p->set_proc_is_defined();
                      if (($3)->is_empty()){
                        printf("KKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n");
                        fflush(stdout);
                      }
                      p->set_local_list(*($7));
                      p->set_ast_list(*($8));
                      if(curr_return_type != curr_fn_ret_type){
                        printf("cs316: Error Return type not matching\n");
                        exit(1);
                      }
                      curr_return_type = void_data_type;
                      local_t = new Symbol_Table();
                    }
        | VOID NAME '(' narg_list ')'
                      {
                        string us("_");
                        string* sss = new string(*($2) + us);
                        if(program_object.is_procedure_exists(*sss)){
					              	printf("cs316: Error Current procedure already defined \n");
                          exit(1);
					              }
                        p = new Procedure(void_data_type, *sss, lineno);
                        p->set_proc_is_defined();
                        p->set_formal_param_list(*$4);
                        program_object.set_proc_to_map(*sss, p);
                        curr_name = *sss;
                        curr_fn_ret_type = void_data_type;
                      }
                          '{' dl2 sl '}'
                    {
                      p->set_local_list(*($8));
                      p->set_ast_list(*($9));
                      if(curr_return_type != curr_fn_ret_type){
                        printf("cs316: Error Return type not matching\n");
                        exit(1);
                      }
                      curr_return_type = void_data_type;
                      local_t = new Symbol_Table();
                    }                      
        ;



narg_list: /* empty */ { Symbol_Table *s1 = new Symbol_Table(); $$ = s1;}
          | arg_list {$$ = $1;}
          ;

arg_list : argu { Symbol_Table *s1 = new Symbol_Table(); s1->push_symbol($1); $$ = s1;}
         | arg_list ',' argu {$1->push_symbol($3); $$ = $1; }
         ;

argu    : type NAME
          {
            string *s = new string(*$2 + "_");
            Symbol_Table_Entry *se = new Symbol_Table_Entry(*s, $1, lineno);
            se->set_data_type($1);
            $$ = se;
          }
        ;

pd      : VOID MAIN '(' ')' '{' dl2 sl '}'
          {
            fflush(stdout);
            string main_str("main");
            p = new Procedure(void_data_type, main_str, lineno);
            p->set_local_list(*($6));
            p->set_ast_list(*($7));
            $$ = p;
          }
        ;
sl      : /*empty */  {list<Ast *> *l = new list<Ast *>();$$ = l;}
        | sl ans
        {
          ($1)->push_back($2);
          $$ = $1;
        }
        | sl bblock
        {
          ($1)->push_back($2);
          $$ = $1;
        }
        ;
bblock  : '{' nsl '}'  {$$ = $2;}
        ;
nsl     : ans  
        {
          Sequence_Ast *s = new Sequence_Ast(lineno);
          s->ast_push_back($1);
          $$ = s;
        }
        | bblock
        {
          Sequence_Ast *s = new Sequence_Ast(lineno);
          s->ast_push_back($1);
          $$ = s;
        }
        | nsl ans
        {
          ($1)->ast_push_back($2);
          $$ = $1;
        }
        | nsl bblock
        {
          ($1)->ast_push_back($2);
          $$ = $1;
        }
        ;
temp    : '{' nsl '}' {$$ = $2;}
        | ans 
        ;

temp1   : '{' nsl '}' {$$ = $2;}
        | mifstmt 
        | ans1 
        ;

mifstmt : IF '(' cond ')' temp1 ELSE temp1
        {
          Selection_Statement_Ast *s = new Selection_Statement_Ast($3,$5, $7, lineno);
          $$ = s;
        }
        ;

uifstmt : IF '(' cond ')' temp1
        {
          Selection_Statement_Ast *s = new Selection_Statement_Ast($3,$5, NULL, lineno);
          $$ = s;
        }
        | IF '(' cond ')' uifstmt
        {
          Selection_Statement_Ast *s = new Selection_Statement_Ast($3,$5, NULL, lineno);
          $$ = s;
        }
        | IF '(' cond ')' temp1 ELSE uifstmt
        {
          Selection_Statement_Ast *s = new Selection_Statement_Ast($3,$5, $7, lineno);
          $$ = s;
        }
        ;

ifstmt  : mifstmt
        | uifstmt
        ;
ans     : ifstmt
        | ans1
        ;
ans1    : WHILE '(' cond ')' temp
        {
          Iteration_Statement_Ast *i = new Iteration_Statement_Ast($3, $5, lineno, false);
          $$ = i;
        }
        | DO '{' nsl '}' WHILE '(' cond ')' ';'
        {
          Iteration_Statement_Ast *i = new Iteration_Statement_Ast($7, $3, lineno, true);
          $$ = i;
        }
        | as
        | ps
        | func_call ';'
        | ret
        ;

///////////////////////////////////////////////////////

cond    : cond OR cond
        {
          Logical_Expr_Ast *l = new Logical_Expr_Ast($1,_logical_or,$3,lineno);
          l->set_data_type(int_data_type);
          $$ = l;
        }
        | cond AND cond
        {
          Logical_Expr_Ast *l = new Logical_Expr_Ast($1,_logical_and,$3,lineno);
          l->set_data_type(int_data_type);
          $$ = l;
        }
        | NOT cond
        {
          Logical_Expr_Ast *l = new Logical_Expr_Ast(NULL,_logical_not,$2,lineno);
          l->set_data_type(int_data_type);
          $$ = l;
        }
        | cstmt 
        | '(' cond ')' {$$ = $2;}

///////////////////////////////////////////////////////
// cond    : cond OR cond1
//         {
//           Logical_Expr_Ast *l = new Logical_Expr_Ast($1,_logical_or,$3,lineno);
//           $$ = l;
//         }
//         | cond1
//         ;
// cond1   : cond1 AND cond2
//         {
//           printf("Reduced cond1\n");
//           fflush(stdout);
//           Logical_Expr_Ast *l = new Logical_Expr_Ast($1,_logical_and,$3,lineno);
//           $$ = l;
//         }
//         | cond2
//         ;
// cond2   : NOT cond2 
//         {
//           printf("Reduced cond2\n");
//           fflush(stdout);
//           Logical_Expr_Ast *l = new Logical_Expr_Ast(NULL,_logical_not,$2,lineno);
//           $$ = l;
//         }
//         | cstmt { printf("Reduced cond3\n"); fflush(stdout);}
//         | '(' cond ')' {$$ = $2;}
//         ;
///////////////////////////////////////////////////////

cstmt   : expr0 relop expr0
        {
          Relational_Expr_Ast *r = new Relational_Expr_Ast($1, $2, $3, lineno);
          r->set_data_type(($1)->get_data_type());
          r->check_ast(); 
          $$ = r;
        }
        ;
relop   : E {$$ = equalto;}
        | NE {$$ = not_equalto;}
        | GTE {$$ = greater_equalto;}
        | LTE {$$ = less_equalto;}
        | LT {$$ = less_than;}
        | GT {$$ = greater_than;}
        ;

ps      : PRINT NAME ';'
          {
            string us("_");
            string sss = *($2) + us;
            Name_Ast *n;
            if(local_t->variable_in_symbol_list_check(sss))
            { 
              n = new Name_Ast(sss,local_t->get_symbol_table_entry(sss),lineno);
            }
            else if(global_t->variable_in_symbol_list_check(sss))
            { 
              n = new Name_Ast(sss,global_t->get_symbol_table_entry(sss),lineno);
            }
            ////////////////////////////////
            else if(p != NULL){
              Symbol_Table sym_ttt = p->get_formal_param_list();
              if (sym_ttt.variable_in_symbol_list_check(sss)){
                n = new Name_Ast(sss,sym_ttt.get_symbol_table_entry(sss),lineno);
              }
            }
            else
            {
              printf("cs316: Error Variable Not Declared\n");
            }
            ////////////////////////////////
            Print_Ast *past = new Print_Ast(n, lineno);
            $$ = past;
          }

func_call : NAME '(' eparam_list ')'
          {
            string us("_");
            string sss = *($1) + us;
            Call_Ast * c1 = new Call_Ast(sss, lineno);
					  c1->set_actual_param_list(*$3);
					  $$ = c1;
          }
          ;

eparam_list: /*empty */ {list<Ast*> *l1 = new list<Ast*>(); $$ = l1;}
            | param_list {$$ = $1; }
            ;

param_list : expr0
            {
				      list<Ast*> *l1 = new list<Ast*>();
              l1->push_back($1);
				      $$ = l1;
			      }
            | param_list ',' expr0
            {
              ($1)->push_back($3);
              $$ = $1;
            }
            ;

ret     : RETURN ';'
          {
		  		  Return_Ast* r1 = new Return_Ast(NULL, curr_name, lineno);
            curr_return_type = void_data_type;
		  		  $$ = r1;
		      }
		      | RETURN expr0 ';'
          {
		      	Return_Ast* r1 = new Return_Ast($2, curr_name, lineno);
            curr_return_type = ($2)->get_data_type();
		      	$$ = r1;
		      }	

as      : NAME ASSIGN expr0 ';'
          {
            string us("_");
            string sss = *($1) + us;
            Name_Ast *n;
            if(local_t->variable_in_symbol_list_check(sss))
            { 
              n = new Name_Ast(sss,local_t->get_symbol_table_entry(sss),lineno);
            }
            else if(global_t->variable_in_symbol_list_check(sss))
            { 
              n = new Name_Ast(sss,global_t->get_symbol_table_entry(sss),lineno);
            }
            else if(p != NULL){
              Symbol_Table sym_ttt = p->get_formal_param_list();
              if (sym_ttt.variable_in_symbol_list_check(sss)){
                n = new Name_Ast(sss,sym_ttt.get_symbol_table_entry(sss),lineno);
              }
            }
            ////////////////////////////////
            else
            {
              printf("cs316: Error Variable Not Declared\n");
            }
            ////////////////////////////////
            Assignment_Ast *a = new Assignment_Ast(n,$3,lineno);
            a->set_data_type(n->get_data_type());
            /**/  a->check_ast();  /**/;
            $$ = a;
          }

expr0   : cond '?' expr0 ':' expr0
        {
          Conditional_Expression_Ast *c = new Conditional_Expression_Ast($1,$3,$5,lineno);
          c->set_data_type(($3)->get_data_type()); 
          /**/  
          c->check_ast();
          /**/
          $$ = c;
        }
        | expr
        ;
expr    : expr '+' expr1   {Plus_Ast *d = new Plus_Ast($1,$3,lineno); d->set_data_type(($1)->get_data_type()); /**/  d->check_ast();  /**/ $$ = d;}
        | expr '-' expr1   {Minus_Ast *d = new Minus_Ast($1,$3,lineno); d->set_data_type(($1)->get_data_type());  /**/  d->check_ast();  /**/ $$ = d;}
        | expr1
        ;
expr1   : expr1 '*' expr2  {Mult_Ast *d = new Mult_Ast($1,$3,lineno);  d->set_data_type(($1)->get_data_type()); /**/  d->check_ast();  /**/ $$ = d;}
        | expr1 '/' expr2 {Divide_Ast *d = new Divide_Ast($1,$3,lineno); d->set_data_type(($1)->get_data_type()); /**/  d->check_ast();  /**/ $$ = d;}
        | expr2
        ;
expr2   : '-' expr2 {UMinus_Ast *u = new UMinus_Ast($2, NULL, lineno); u->set_data_type(($2)->get_data_type()); $$ = u;}
        | expr3
expr3   : NAME
          {
            string us("_");
            string sss = *($1) + us;
            Name_Ast *n;
            if(local_t->variable_in_symbol_list_check(sss))
            {
              n = new Name_Ast(sss,local_t->get_symbol_table_entry(sss),lineno);
              n->set_data_type(local_t->get_symbol_table_entry(sss).get_data_type());
            }
            else if(global_t->variable_in_symbol_list_check(sss))
            {
              n = new Name_Ast(sss,global_t->get_symbol_table_entry(sss),lineno);
              n->set_data_type(global_t->get_symbol_table_entry(sss).get_data_type());
            }
            else if(p != NULL){
              Symbol_Table sym_ttt = p->get_formal_param_list();
              if (sym_ttt.variable_in_symbol_list_check(sss)){
                n = new Name_Ast(sss,sym_ttt.get_symbol_table_entry(sss),lineno);
              }
            }
            ////////////////////////////////
            else
            {
              printf("cs316: Error Variable Not Declared\n");
            }
            ////////////////////////////////
            $$ = n;
          }
        | func_call  {$$ = $1; }
        | INTEGER_NUMBER   {Number_Ast<int> *n = new Number_Ast<int>($1,int_data_type,lineno); n->set_data_type(int_data_type);  $$ = n;}
        | DOUBLE_NUMBER    {Number_Ast<double> *n = new Number_Ast<double>($1,double_data_type,lineno); n->set_data_type(double_data_type);  $$ = n;}
        | '(' expr0 ')'  {$$ = $2;}
        ;
