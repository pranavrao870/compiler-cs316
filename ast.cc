#include <string.h>
#include <string>
#include <sstream>

template class Number_Ast<double>;
template class Number_Ast<int>;
using namespace std;
Ast::Ast(){}
Ast::~Ast(){}

int Ast::labelCounter = 0;

Data_Type Ast::get_data_type(){
    return node_data_type;
}

void Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Ast::is_value_zero(){
    return false;
}

bool Ast::check_ast(){
    return false;
}

Symbol_Table_Entry & Ast::get_symbol_entry(){
    Symbol_Table_Entry variable_symbol_entry;
    return variable_symbol_entry;
}

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
    return;
}

////////////////////////////////////////////////////////////

Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
    lineno = line;
    ast_num_child = binary_arity;
    lhs = temp_lhs;
    rhs = temp_rhs;
}

Assignment_Ast::~Assignment_Ast(){
    delete lhs;
    delete rhs;
}

bool Assignment_Ast::check_ast(){
    Data_Type ld = lhs->get_data_type();
    Data_Type rd = rhs->get_data_type();
    if (ld == rd){
        return true;
    }
    else{
        fprintf(stderr, "cs316: Error Line: %d: Assignment statement data type not compatible\n", lineno);
        return false;
    }
}

void Assignment_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n         Asgn:\n            LHS (";
    file_buffer.write(to_write1, strlen(to_write1));
    lhs->print(file_buffer);
    char *to_write2 = ")\n            RHS (";
    file_buffer.write(to_write2, strlen(to_write2));
    rhs->print(file_buffer);
    char *to_write3 = ")";
    file_buffer.write(to_write3, strlen(to_write3));
}

/////////////////////////////////////////////////////////////////

Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
    lineno = line;
    ast_num_child = zero_arity;
    variable_symbol_entry = &var_entry;
}

Name_Ast::~Name_Ast(){}

Data_Type Name_Ast::get_data_type(){
    node_data_type = variable_symbol_entry->get_data_type();
    return node_data_type;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
    return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

void Name_Ast::print(ostream & file_buffer){
    string tmp = "Name : " + variable_symbol_entry->get_variable_name();
    file_buffer.write(tmp.c_str(), tmp.size());
}

//////////////////////////////////////////////////////////////////////
template<class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
    lineno = line;
    ast_num_child = zero_arity;
    constant = number;
    node_data_type = constant_data_type;
}

template<class T>
Number_Ast<T>::~Number_Ast(){}

template<class T>
Data_Type Number_Ast<T>::get_data_type(){
    return node_data_type;
}

template<class T>
void Number_Ast<T>::set_data_type(Data_Type dt){
    node_data_type = dt;
}

template<class T>
bool Number_Ast<T>::is_value_zero(){
    return (constant == 0);
}

template<class T>
void Number_Ast<T>::print(ostream & file_buffer){
    // ostringstream cons_str;
    // cons_str.precision(2);
    // cons_str << constant;
    // string tmp = "Num : " + cons_str.str();
    // file_buffer.write(tmp.c_str(), tmp.size());
    char buf[200];
    sprintf(buf, "Num : %d", constant);
    file_buffer.write(buf, strlen(buf));
}

template <>
void Number_Ast<double>::print(ostream & file_buffer){
    char buf[200];
    sprintf(buf, "Num : %0.2f", constant);
    file_buffer.write(buf, strlen(buf));
}

/////////////////////////////////////////////////////////////////////////

Data_Type Arithmetic_Expr_Ast::get_data_type(){
    node_data_type = lhs->get_data_type();
    return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast(){
    Data_Type ld = lhs->get_data_type();
    if (rhs == NULL){
        return true;
    }
    Data_Type rd = rhs->get_data_type();
    if (ld == rd){
        return true;
    }
    else{
        fprintf(stderr, "cs316: Error Line: %d: Arithmetic statement data type not compatible\n", lineno);
        return false;
    }
}

/////////////////////////////////////////////////////////////////////////

Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
    lineno = line;
    ast_num_child = binary_arity;
    lhs = l;
    rhs = r;
}

void Plus_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n            Arith: PLUS\n               LHS (";
    file_buffer.write(to_write1, strlen(to_write1));
    lhs->print(file_buffer);
    char *to_write2 = ")\n               RHS (";
    file_buffer.write(to_write2, strlen(to_write2));
    rhs->print(file_buffer);
    char *to_write3 = ")";
    file_buffer.write(to_write3, strlen(to_write3));
}

///////////////////////////////////////////////////////////////////////

Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
    lineno = line;
    ast_num_child = binary_arity;
    lhs = l;
    rhs = r;
}

void Minus_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n            Arith: MINUS\n               LHS (";
    file_buffer.write(to_write1, strlen(to_write1));
    lhs->print(file_buffer);
    char *to_write2 = ")\n               RHS (";
    file_buffer.write(to_write2, strlen(to_write2));
    rhs->print(file_buffer);
    char *to_write3 = ")";
    file_buffer.write(to_write3, strlen(to_write3));
}

//////////////////////////////////////////////////////////////////////////

Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
    lineno = line;
    ast_num_child = binary_arity;
    lhs = l;
    rhs = r;
}

void Mult_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n            Arith: MULT\n               LHS (";
    file_buffer.write(to_write1, strlen(to_write1));
    lhs->print(file_buffer);
    char *to_write2 = ")\n               RHS (";
    file_buffer.write(to_write2, strlen(to_write2));
    rhs->print(file_buffer);
    char *to_write3 = ")";
    file_buffer.write(to_write3, strlen(to_write3));
}

/////////////////////////////////////////////////////////////////////////////

Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
    lineno = line;
    ast_num_child = binary_arity;
    lhs = l;
    rhs = r;
}

void Divide_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n            Arith: DIV\n               LHS (";
    file_buffer.write(to_write1, strlen(to_write1));
    lhs->print(file_buffer);
    char *to_write2 = ")\n               RHS (";
    file_buffer.write(to_write2, strlen(to_write2));
    rhs->print(file_buffer);
    char *to_write3 = ")";
    file_buffer.write(to_write3, strlen(to_write3));
}

/////////////////////////////////////////////////////////////////////////////

UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
    lineno = line;
    ast_num_child = unary_arity;
    lhs = l;
    rhs = r;
}

void UMinus_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n            Arith: UMINUS\n               LHS (";
    file_buffer.write(to_write1, strlen(to_write1));
    lhs->print(file_buffer);
    char *to_write3 = ")";
    file_buffer.write(to_write3, strlen(to_write3));
}

///////////////////////////////////////////////////////////////////////////

Conditional_Expression_Ast::Conditional_Expression_Ast(Ast* cond, Ast* l, Ast* r, int line){
    labelCounter ++;
    lineno = line;
    ast_num_child = ternary_arity;
    lhs = l;
    rhs = r;
    this->cond = cond;
}

void Conditional_Expression_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n         Cond:\n            IF_ELSE";
    file_buffer.write(to_write1, strlen(to_write1));
    cond->print(file_buffer);
    char *to_write2 = "\n            LHS (";
    file_buffer.write(to_write2, strlen(to_write2));
    lhs->print(file_buffer);
    char *to_write3 = ")\n            RHS (";
    file_buffer.write(to_write3, strlen(to_write3));
    rhs->print(file_buffer);
    char *to_write4 = ")";
    file_buffer.write(to_write4, strlen(to_write4));
}

///////////////////////////////////////////////////////////////////////////

Relational_Expr_Ast::Relational_Expr_Ast(Ast * lhs, Relational_Op rop, Ast * rhs, int line){
    lineno = line;
    ast_num_child = binary_arity;
    lhs_condition = lhs;
    rhs_condition = rhs;
    rel_op = rop;
}

Data_Type Relational_Expr_Ast::get_data_type(){
    node_data_type = lhs_condition->get_data_type();
    return node_data_type;
}

void Relational_Expr_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast(){
    Data_Type ld = lhs_condition->get_data_type();
    Data_Type rd = rhs_condition->get_data_type();
    if (ld == rd){
        return true;
    }
    else{
        fprintf(stderr, "cs316: Error Line: %d: Relational statement data type not compatible\n", lineno);
        return false;
    }
}

void Relational_Expr_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n            Condition: ";
    file_buffer.write(to_write1, strlen(to_write1));
    if(rel_op==less_equalto){
        char *to_write5  = "LE";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    else if(rel_op==less_than){
        char *to_write5  = "LT";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    else if(rel_op==greater_than){
        char *to_write5  = "GT";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    else if(rel_op==greater_equalto){
        char *to_write5  = "GE";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    else if(rel_op==equalto){
        char *to_write5  = "EQ";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    else{
        char *to_write5  = "NE";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    char *to_write2 = "\n               LHS (";
    file_buffer.write(to_write2, strlen(to_write2));
    lhs_condition->print(file_buffer);
    char *to_write3 = ")\n               RHS (";
    file_buffer.write(to_write3, strlen(to_write3));
    rhs_condition->print(file_buffer);
    char *to_write4 = ")";
    file_buffer.write(to_write4, strlen(to_write4));
}

///////////////////////////////////////////////////////////////////////////

Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
    lhs_op = lhs;
    rhs_op = rhs;
    bool_op = bop;
    lineno = line;
    ast_num_child = binary_arity;
}

Data_Type Logical_Expr_Ast::get_data_type(){
    return int_data_type;
}

void Logical_Expr_Ast::set_data_type(Data_Type dt){
}

bool Logical_Expr_Ast::check_ast(){
    return true;
}

void Logical_Expr_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n            Condition: ";
    file_buffer.write(to_write1, strlen(to_write1));
    if(bool_op==_logical_or){
        char *to_write5  = "OR";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    else if(bool_op==_logical_and){
        char *to_write5  = "AND";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    else{
        char *to_write5  = "NOT";
        file_buffer.write(to_write5, strlen(to_write5));
    }
    if(bool_op!=_logical_not){
        char *to_write2 = "\n               LHS (";
        file_buffer.write(to_write2, strlen(to_write2));
        lhs_op->print(file_buffer);
        char *to_write6 = ")";
        file_buffer.write(to_write6, strlen(to_write6));
    }
    char *to_write3 = "\n               RHS (";
    file_buffer.write(to_write3, strlen(to_write3));
    rhs_op->print(file_buffer);
    char *to_write4 = ")";
    file_buffer.write(to_write4, strlen(to_write4));
}

///////////////////////////////////////////////////////////////////////////
Selection_Statement_Ast::Selection_Statement_Ast(Ast * cond,Ast* then_part, Ast* else_part, int line){
    this->cond = cond;
    this->then_part = then_part;
    this->else_part = else_part;
    lineno = line;
    ast_num_child = ternary_arity;
}

Selection_Statement_Ast::~Selection_Statement_Ast(){
    delete cond;
    delete then_part;
    delete else_part;
}

Data_Type Selection_Statement_Ast::get_data_type(){
    return int_data_type;
}

void Selection_Statement_Ast::set_data_type(Data_Type dt){
}

bool Selection_Statement_Ast::check_ast(){
    return true;
}

void Selection_Statement_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n         IF : \n         CONDITION (";
    file_buffer.write(to_write1, strlen(to_write1));
    cond->print(file_buffer);
    char *to_write2 = ")\n         THEN (";
    file_buffer.write(to_write2, strlen(to_write2));
    then_part->print(file_buffer);
    if(else_part != NULL){
        char *to_write3 = ")\n         ELSE (";
        file_buffer.write(to_write3, strlen(to_write3));
        else_part->print(file_buffer);
    }
    char *to_write4 = ")";
    file_buffer.write(to_write4, strlen(to_write4));
}

///////////////////////////////////////////////////////////////////////////

Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * cond, Ast* body, int line, bool do_form){
    this->cond = cond;
    this->body = body;
    is_do_form = do_form;
    lineno = line;
    ast_num_child = binary_arity;
}

Iteration_Statement_Ast::~Iteration_Statement_Ast(){
    delete cond;
    delete body;
}

Data_Type Iteration_Statement_Ast::get_data_type(){
    return int_data_type;
}

void Iteration_Statement_Ast::set_data_type(Data_Type dt){
}

bool Iteration_Statement_Ast::check_ast(){
    return true;
}

void Iteration_Statement_Ast::print(ostream & file_buffer){
    if(is_do_form){
        char *to_write1  = "\n         DO (";
        file_buffer.write(to_write1, strlen(to_write1));
        body->print(file_buffer);
        char *to_write2 = ")\n         WHILE CONDITION (";
        file_buffer.write(to_write2, strlen(to_write2));
        cond->print(file_buffer);
        char *to_write3 = ")";
        file_buffer.write(to_write3, strlen(to_write3));
    }
    else{
        char *to_write1  = "\n         WHILE : \n         CONDITION (";
        file_buffer.write(to_write1, strlen(to_write1));
        cond->print(file_buffer);
        char *to_write2 = ")\n         BODY (";
        file_buffer.write(to_write2, strlen(to_write2));
        body->print(file_buffer);
        char *to_write3 = ")";
        file_buffer.write(to_write3, strlen(to_write3));
    }
}

///////////////////////////////////////////////////////////////////////////

Sequence_Ast::Sequence_Ast(int line){
    lineno = line;
    labelCounter++;
}

void Sequence_Ast::ast_push_back(Ast * ast){
    statement_list.push_back(ast);
}

void Sequence_Ast::print(ostream & file_buffer){
    for (list<Ast *>::iterator it = statement_list.begin(); it != statement_list.end(); ++it){
        char *to_write1  = "\n            ";
        file_buffer.write(to_write1, strlen(to_write1));
        (*it)->print(file_buffer);
    }
}

///////////////////////////////////////////////////////////////////////////

Print_Ast::Print_Ast(Ast *v, int line){
    var = v;
    lineno = line;
}

Print_Ast::~Print_Ast() {}

void Print_Ast::print(ostream & file_buffer){
    char *to_write1  = "\n         Print : \n               (";
    file_buffer.write(to_write1, strlen(to_write1));
    var->print(file_buffer);
    char *to_write2 = ")'\n";
    file_buffer.write(to_write2, strlen(to_write2));
}

///////////////////////////////////////////////////////////////////////////

Return_Ast::Return_Ast(Ast* ret_val, string name, int line){
	return_value = ret_val;
	proc_name = name;
	lineno = line;
}

Return_Ast::~Return_Ast() {}

Data_Type Return_Ast::get_data_type(){
	if (return_value == NULL){
        return void_data_type;
    }
    return return_value->get_data_type();
}

void Return_Ast::print(ostream & file_buffer) {}

Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {}

///////////////////////////////////////////////////////////////////////////

Call_Ast::Call_Ast(string name, int line){
	procedure_name = name;
	lineno = line;
	node_data_type = program_object.get_procedure_prototype(procedure_name)->get_return_type();
	if(node_data_type == int_data_type){
		return_value_reg = machine_desc_object.spim_register_table[v1];
	}
	else if(node_data_type == double_data_type){
		return_value_reg = machine_desc_object.spim_register_table[f0];
	}
}

Data_Type Call_Ast::get_data_type(){
	return node_data_type;
}

void Call_Ast::set_register(Register_Descriptor * reg){
	return_value_reg = reg;
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list) {}

void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
	actual_param_list = param_list;
}

void Call_Ast::print(ostream & file_buffer) {}

Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer) {}

///////////////////////////////////////////////////////////////////////////