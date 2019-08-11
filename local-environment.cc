#include <string>
#include <map>
#include <string.h>

using namespace std;

////////////////////////////////////////////////////////////

int Eval_Result::get_int_value(){
	return 0;
}

void Eval_Result::set_value(int value){}

double Eval_Result::get_double_value(){
	return 0.0;
}

void Eval_Result::set_value(double value){}

bool Eval_Result::is_variable_defined(){
	return false;
}

void Eval_Result::set_variable_status(bool def){}

// void Eval_Result::set_result_enum(Result_Enum res){
// 	result_type = res;
// }

// Result_Enum Eval_Result::get_result_enum(){
// 	return result_type;
// }

////////////////////////////////////////////////////////////

int Eval_Result_Value::get_int_value(){
	return 0;
}

void Eval_Result_Value::set_value(int number){}

double Eval_Result_Value::get_double_value(){
	return 0.0;
}

void Eval_Result_Value::set_value(double number){}

// bool Eval_Result_Value::is_variable_defined(){
// 	return false;
// }

// void Eval_Result_Value::set_variable_status(bool def){}

// void Eval_Result_Value::set_result_enum(Result_Enum res){
// 	result_type = res;
// }

// Result_Enum Eval_Result_Value::get_result_enum(){
// 	return result_type;
// }

////////////////////////////////////////////////////////////

Eval_Result_Value_Int::Eval_Result_Value_Int(){}

Eval_Result_Value_Int::~Eval_Result_Value_Int(){}

int Eval_Result_Value_Int::get_int_value(){
	return value;
}

void Eval_Result_Value_Int::set_value(int number){
	this->value = number;
}

bool Eval_Result_Value_Int::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Int::set_variable_status(bool def){
	defined = def;
}

void Eval_Result_Value_Int::set_value(double number){}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res){
	result_type = res;
}

Result_Enum Eval_Result_Value_Int::get_result_enum(){
	return result_type;
}

////////////////////////////////////////////////////////////
Eval_Result_Value_Double::Eval_Result_Value_Double(){}

Eval_Result_Value_Double::~Eval_Result_Value_Double(){}

double Eval_Result_Value_Double::get_double_value(){
	return value;
}

void Eval_Result_Value_Double::set_value(int number){}

bool Eval_Result_Value_Double::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Double::set_variable_status(bool def){
	defined = def;
}

void Eval_Result_Value_Double::set_value(double number){
	this->value = number;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res){
	result_type = res;
}

Result_Enum Eval_Result_Value_Double::get_result_enum(){
	return result_type;
}

////////////////////////////////////////////////////////////

Local_Environment::Local_Environment(){}

Local_Environment::~Local_Environment(){}

void Local_Environment::print(ostream & file_buffer){
	for(map<string,Eval_Result *>::iterator it = variable_table.begin(); it != variable_table.end(); ++it){
		char *to_write1  = "         ";
	    file_buffer.write(to_write1, strlen(to_write1));
		string tmp = it->first;
	    file_buffer.write(tmp.c_str(), tmp.size());
	    if(it->second->is_variable_defined()){
	    	if(it->second->get_result_enum()==int_result){
				char buf[200];
			    sprintf(buf, " : %d", it->second->get_int_value());
			    file_buffer.write(buf, strlen(buf));
			}
			else{
				char buf[200];
			    sprintf(buf, " : %0.2f", it->second->get_double_value());
			    file_buffer.write(buf, strlen(buf));
			}
	    }
	    else{
	    	char *to_write2  = " : undefined";
	    	file_buffer.write(to_write2, strlen(to_write2));
	    }
	    char *to_write3  = "\n";
	    file_buffer.write(to_write3, strlen(to_write3));
	}
}

bool Local_Environment::is_variable_defined(string name){
	return variable_table[name]->is_variable_defined();
}

bool Local_Environment::does_variable_exist(string name){
	if(variable_table.find(name)!=variable_table.end()) return true;
	else return false;
}

Eval_Result * Local_Environment::get_variable_value(string name){
	return variable_table[name];
}

void Local_Environment::put_variable_value(Eval_Result & value, string name){
	variable_table[name] = &value;
}

////////////////////////////////////////////////////////////

void Symbol_Table::create(Local_Environment & local_global_variables_table){
	if(scope==global){
		for(list<Symbol_Table_Entry *>::iterator it = variable_table.begin(); it!=variable_table.end(); it++){
			if((*it)->get_data_type()==int_data_type){
				Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
				e->set_variable_status(true);
				int a = 0;
				e->set_value(a);
				e->set_result_enum(int_result);
				local_global_variables_table.put_variable_value(*e,(*it)->get_variable_name());
			}
			else{
				Eval_Result_Value_Double *e = new Eval_Result_Value_Double();
				e->set_variable_status(true);
				double a = 0;
				e->set_value(a);
				e->set_result_enum(double_result);
				local_global_variables_table.put_variable_value(*e,(*it)->get_variable_name());
			}
		}
	}
	else{
		for(list<Symbol_Table_Entry *>::iterator it = variable_table.begin(); it!=variable_table.end(); it++){
			if((*it)->get_data_type()==int_data_type){
				Eval_Result_Value_Int *e = new Eval_Result_Value_Int();
				e->set_variable_status(false);
				e->set_result_enum(int_result);
				local_global_variables_table.put_variable_value(*e,(*it)->get_variable_name());
			}
			else{
				Eval_Result_Value_Double *e = new Eval_Result_Value_Double();
				e->set_variable_status(false);
				e->set_result_enum(double_result);
				local_global_variables_table.put_variable_value(*e,(*it)->get_variable_name());
			}
		}
	}
}

////////////////////////////////////////////////////////////