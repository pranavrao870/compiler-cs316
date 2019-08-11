#include <string.h>
#include <string>
#include <sstream>
#include "local-environment.hh"

using namespace std;
template class Number_Ast<double>;
template class Number_Ast<int>;


Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){
	Eval_Result_Value_Int e;
	return e;
}
void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){}

////////////////////////////////////////////////////////////

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	print(file_buffer);
	Eval_Result *e = &(rhs->evaluate(eval_env,file_buffer));
	lhs->set_value_of_evaluation(eval_env,*e);
	lhs->print_value(eval_env,file_buffer);
	return *e;
}

////////////////////////////////////////////////////////////

Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
	string name = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(name)){
		//////////////////////////////////////////////////////////////////////
		if(!eval_env.get_variable_value(name)->is_variable_defined()){
			fprintf(stderr, "cs316: Error Line: %d: Varible Not Defined\n", lineno);
			exit(1);
		}
		//////////////////////////////////////////////////////////////////////
		else{
			return *eval_env.get_variable_value(name);
		}
	}
	else{
		return *interpreter_global_table.get_variable_value(name);
	}
}
void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
	if(result.get_result_enum()==int_result){
		int a = result.get_int_value();
		string name = variable_symbol_entry->get_variable_name();
		if(eval_env.does_variable_exist(name)){
			eval_env.get_variable_value(name)->set_value(a);
			eval_env.get_variable_value(name)->set_variable_status(true);
		}
		else{
			interpreter_global_table.get_variable_value(name)->set_value(a);
		}
	}
	else{
		double a = result.get_double_value();
		string name = variable_symbol_entry->get_variable_name();
		if(eval_env.does_variable_exist(name)){
			eval_env.get_variable_value(name)->set_value(a);
			eval_env.get_variable_value(name)->set_variable_status(true);
		}
		else{
			interpreter_global_table.get_variable_value(name)->set_value(a);
		}
	}
}
Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	string name = variable_symbol_entry->get_variable_name();
	if(eval_env.does_variable_exist(name)){
		//////////////////////////////////////////////////////////////////////
		if(!eval_env.get_variable_value(name)->is_variable_defined()){
			fprintf(stderr, "cs316: Error Line: %d: Varible Not Defined\n", lineno);
			exit(1);
		}
		//////////////////////////////////////////////////////////////////////
		else{
			return *eval_env.get_variable_value(name);
		}
	}
	else{
		return *interpreter_global_table.get_variable_value(name);
	}
}
void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
	string name = variable_symbol_entry->get_variable_name();
	Eval_Result *e;
	if(eval_env.does_variable_exist(name)){
		e = eval_env.get_variable_value(name);
	}
	else{
		e = interpreter_global_table.get_variable_value(name);
	}
	char *to_write1  = "\n         ";
    file_buffer.write(to_write1, strlen(to_write1));
	string tmp = name;
    file_buffer.write(tmp.c_str(), tmp.size());
	if(e->get_result_enum()==int_result){
		char buf[200];
	    sprintf(buf, " : %d", e->get_int_value());
	    file_buffer.write(buf, strlen(buf));
	}
	else{
		char buf[200];
	    sprintf(buf, " : %0.2f", e->get_double_value());
	    file_buffer.write(buf, strlen(buf));
	}
	char *to_write2  = "\n\n";
    file_buffer.write(to_write2, strlen(to_write2));
}

////////////////////////////////////////////////////////////

template<class T>
Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
		e->set_result_enum(int_result);
		e->set_value(constant);
		e->set_variable_status(true);
		return *e;
	}
	else{
		Eval_Result_Value_Double *e = new Eval_Result_Value_Double();
		e->set_result_enum(double_result);
		e->set_value(constant);
		e->set_variable_status(true);
		return *e;
	}
}

////////////////////////////////////////////////////////////

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
		e->set_result_enum(int_result);
		int x = lhs->evaluate(eval_env,file_buffer).get_int_value()+rhs->evaluate(eval_env,file_buffer).get_int_value();
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
	else{
		Eval_Result_Value_Double *e = new Eval_Result_Value_Double();
		e->set_result_enum(double_result);
		double x = lhs->evaluate(eval_env,file_buffer).get_double_value()+rhs->evaluate(eval_env,file_buffer).get_double_value();
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
}

////////////////////////////////////////////////////////////

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
		e->set_result_enum(int_result);
		int x = lhs->evaluate(eval_env,file_buffer).get_int_value()-rhs->evaluate(eval_env,file_buffer).get_int_value();
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
	else{
		Eval_Result_Value_Double *e = new Eval_Result_Value_Double();
		e->set_result_enum(double_result);
		double x = lhs->evaluate(eval_env,file_buffer).get_double_value()-rhs->evaluate(eval_env,file_buffer).get_double_value();
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
}

////////////////////////////////////////////////////////////

Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
		e->set_result_enum(int_result);
		int y = rhs->evaluate(eval_env,file_buffer).get_int_value();
		if(y==0){
			fprintf(stderr, "cs316: Error Line: %d: Division by Zero Not Defined\n", lineno);
			exit(1);
		}
		int x = lhs->evaluate(eval_env,file_buffer).get_int_value()/y;
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
	else{
		Eval_Result_Value_Double *e = new Eval_Result_Value_Double();
		e->set_result_enum(double_result);
		double y = rhs->evaluate(eval_env,file_buffer).get_double_value();
		if(y==0){
			fprintf(stderr, "cs316: Error Line: %d: Division by Zero Not Defined\n", lineno);
			exit(1);
		}
		double x = lhs->evaluate(eval_env,file_buffer).get_double_value()/y;
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
}

////////////////////////////////////////////////////////////

Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
		e->set_result_enum(int_result);
		int x = lhs->evaluate(eval_env,file_buffer).get_int_value()*rhs->evaluate(eval_env,file_buffer).get_int_value();
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
	else{
		Eval_Result_Value_Double *e = new Eval_Result_Value_Double();
		e->set_result_enum(double_result);
		double x = lhs->evaluate(eval_env,file_buffer).get_double_value()*rhs->evaluate(eval_env,file_buffer).get_double_value();
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
}

////////////////////////////////////////////////////////////

Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	if(node_data_type==int_data_type){
		Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
		e->set_result_enum(int_result);
		int x = -1*lhs->evaluate(eval_env,file_buffer).get_int_value();
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
	else{
		Eval_Result_Value_Double *e = new Eval_Result_Value_Double();
		e->set_result_enum(double_result);
		double x = -1*lhs->evaluate(eval_env,file_buffer).get_double_value();
		e->set_value(x);
		e->set_variable_status(true);
		return *e;
	}
}

////////////////////////////////////////////////////////////

Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	int a = cond->evaluate(eval_env,file_buffer).get_int_value();
	if(a)
		return lhs->evaluate(eval_env,file_buffer);
	else
		return rhs->evaluate(eval_env,file_buffer);
}

////////////////////////////////////////////////////////////

Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
	e->set_result_enum(int_result);
	int x=0;
	double a,b;
	if(node_data_type==int_data_type){
		a = lhs_condition->evaluate(eval_env,file_buffer).get_int_value();
		b = rhs_condition->evaluate(eval_env,file_buffer).get_int_value();
	}
	else{
		a = lhs_condition->evaluate(eval_env,file_buffer).get_double_value();
		b = rhs_condition->evaluate(eval_env,file_buffer).get_double_value();
	}
	if(rel_op==less_equalto){
		if(a<=b) x=1;
    }
    else if(rel_op==less_than){
        if(a<b) x=1;
    }
    else if(rel_op==greater_than){
        if(a>b) x=1;
    }
    else if(rel_op==greater_equalto){
        if(a>=b) x=1;
    }
    else if(rel_op==equalto){
        if(a==b) x=1;
    }
    else{
        if(a!=b) x=1;
    }
    e->set_value(x);
    e->set_variable_status(true);
    return *e;
}

////////////////////////////////////////////////////////////

Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
	e->set_result_enum(int_result);
	int x=0;
	int a,b;
	if(lhs_op!=NULL)
		a = lhs_op->evaluate(eval_env,file_buffer).get_int_value();
	b = rhs_op->evaluate(eval_env,file_buffer).get_int_value();
	if(bool_op==_logical_or){
		if(a || b) x=1;
    }
    else if(bool_op==_logical_and){
        if(a && b) x=1;
    }
    else{
        if(!b) x=1;
    }
    e->set_value(x);
    e->set_variable_status(true);
    return *e;
}

////////////////////////////////////////////////////////////

Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	int a = cond->evaluate(eval_env,file_buffer).get_int_value();
	if(a)
		return then_part->evaluate(eval_env,file_buffer);
	else if(else_part!=NULL)
		return else_part->evaluate(eval_env,file_buffer);
	else{
		Eval_Result_Value_Int e;
		return e;
	}
}

////////////////////////////////////////////////////////////

Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *e;
	int c;
	if(is_do_form)
		e = &(body->evaluate(eval_env,file_buffer));
	c = cond->evaluate(eval_env,file_buffer).get_int_value();
	while(c){
		e = &(body->evaluate(eval_env,file_buffer));
		c = cond->evaluate(eval_env,file_buffer).get_int_value();
	}
	return *e;
}

////////////////////////////////////////////////////////////

Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
	Eval_Result *e;
	for (list<Ast *>::iterator it = statement_list.begin(); it != statement_list.end(); ++it){
        e = &((*it)->evaluate(eval_env,file_buffer));
    }
    return *e;
}

////////////////////////////////////////////////////////////