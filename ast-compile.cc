#include <string.h>
#include <string>
#include <sstream>
#include "icode.hh"
#include "reg-alloc.hh"
using namespace std;

template class Number_Ast<double>;
template class Number_Ast<int>;

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Assignment_Ast::compile(){
	Code_For_Ast r = rhs->compile();
	list<Icode_Stmt *> y = r.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r2 = new Register_Addr_Opd(r.get_reg());
	a->set_reg(NULL);
	if(node_data_type == int_data_type){
		Ics_Opd *r3 = new Mem_Addr_Opd(lhs->get_symbol_entry());
		Icode_Stmt * i = new Move_IC_Stmt(store,r3,r2);
		a->append_ics(*i);
	}
	else{
		Ics_Opd *r3 = new Mem_Addr_Opd(lhs->get_symbol_entry());
		Icode_Stmt * i = new Move_IC_Stmt(store_d,r3,r2);
		a->append_ics(*i);
	}
	r.get_reg()->reset_use_for_expr_result();
	return *a;
}

Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Name_Ast::compile(){
	Code_For_Ast * a = new Code_For_Ast();
	Ics_Opd *r1 = new Mem_Addr_Opd(get_symbol_entry());
	if(node_data_type == int_data_type){
		Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
		a->set_reg(temp);
		Ics_Opd *r2 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Move_IC_Stmt(load,r1,r2);
		a->append_ics(*i);
	}
	else{
		Register_Descriptor * temp = machine_desc_object.get_new_register<float_reg>();
		a->set_reg(temp);
		Ics_Opd *r2 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Move_IC_Stmt(load_d,r1,r2);
		a->append_ics(*i);
	}
	return *a;
}

Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
}

Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){
}

//////////////////////////////////////////////////////////////////////

template<class T>
Code_For_Ast & Number_Ast<T>::compile(){
	Code_For_Ast * a = new Code_For_Ast();
	if(node_data_type == int_data_type){
		Ics_Opd *r1 = new Const_Opd<int>(constant);
		Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
		a->set_reg(temp);
		Ics_Opd *r2 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Move_IC_Stmt(imm_load,r1,r2);
		a->append_ics(*i);
	}
	else{
		Ics_Opd *r1 = new Const_Opd<double>(constant);
		Register_Descriptor * temp = machine_desc_object.get_new_register<float_reg>();
		a->set_reg(temp);
		Ics_Opd *r2 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Move_IC_Stmt(imm_load_d,r1,r2);
		a->append_ics(*i);
	}
	return *a;
}

template<class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
}

Code_For_Ast & Plus_Ast::compile(){
	Code_For_Ast l = lhs->compile();
	Code_For_Ast r = rhs->compile();

	list<Icode_Stmt *> x = l.get_icode_list();
	list<Icode_Stmt *> y = r.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
		a->append_ics(*(*it));
	}
	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r1 = new Register_Addr_Opd(l.get_reg());
	Ics_Opd *r2 = new Register_Addr_Opd(r.get_reg());
	if(node_data_type == int_data_type){
		Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Compute_IC_Stmt(add,r1,r2,r3);
		a->append_ics(*i);
	}
	else{
		Register_Descriptor * temp = machine_desc_object.get_new_register<float_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Compute_IC_Stmt(add_d,r1,r2,r3);
		a->append_ics(*i);
	}
	l.get_reg()->reset_use_for_expr_result();
	r.get_reg()->reset_use_for_expr_result();
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
}

Code_For_Ast & Minus_Ast::compile(){
	Code_For_Ast l = lhs->compile();
	Code_For_Ast r = rhs->compile();

	list<Icode_Stmt *> x = l.get_icode_list();
	list<Icode_Stmt *> y = r.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
		a->append_ics(*(*it));
	}
	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r1 = new Register_Addr_Opd(l.get_reg());
	Ics_Opd *r2 = new Register_Addr_Opd(r.get_reg());
	if(node_data_type == int_data_type){
		Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Compute_IC_Stmt(sub,r1,r2,r3);
		a->append_ics(*i);
	}
	else{
		Register_Descriptor * temp = machine_desc_object.get_new_register<float_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Compute_IC_Stmt(sub_d,r1,r2,r3);
		a->append_ics(*i);
	}
	l.get_reg()->reset_use_for_expr_result();
	r.get_reg()->reset_use_for_expr_result();
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
}

Code_For_Ast & Divide_Ast::compile(){
	Code_For_Ast l = lhs->compile();
	Code_For_Ast r = rhs->compile();

	list<Icode_Stmt *> x = l.get_icode_list();
	list<Icode_Stmt *> y = r.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
		a->append_ics(*(*it));
	}
	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r1 = new Register_Addr_Opd(l.get_reg());
	Ics_Opd *r2 = new Register_Addr_Opd(r.get_reg());
	if(node_data_type == int_data_type){
		Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Compute_IC_Stmt(divd,r1,r2,r3);
		a->append_ics(*i);
	}
	else{
		Register_Descriptor * temp = machine_desc_object.get_new_register<float_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Compute_IC_Stmt(div_d,r1,r2,r3);
		a->append_ics(*i);
	}
	l.get_reg()->reset_use_for_expr_result();
	r.get_reg()->reset_use_for_expr_result();
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
}

Code_For_Ast & Mult_Ast::compile(){
	Code_For_Ast l = lhs->compile();
	Code_For_Ast r = rhs->compile();

	list<Icode_Stmt *> x = l.get_icode_list();
	list<Icode_Stmt *> y = r.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
		a->append_ics(*(*it));
	}
	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r1 = new Register_Addr_Opd(l.get_reg());
	Ics_Opd *r2 = new Register_Addr_Opd(r.get_reg());
	if(node_data_type == int_data_type){
		Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Compute_IC_Stmt(mult,r1,r2,r3);
		a->append_ics(*i);
	}
	else{
		Register_Descriptor * temp = machine_desc_object.get_new_register<float_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Compute_IC_Stmt(mult_d,r1,r2,r3);
		a->append_ics(*i);
	}
	l.get_reg()->reset_use_for_expr_result();
	r.get_reg()->reset_use_for_expr_result();
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & UMinus_Ast::compile(){
	Code_For_Ast r = lhs->compile();
	list<Icode_Stmt *> y = r.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r2 = new Register_Addr_Opd(r.get_reg());
	if(node_data_type == int_data_type){
		Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Move_IC_Stmt(uminus,r2,r3);
		a->append_ics(*i);
	}
	else{
		Register_Descriptor * temp = machine_desc_object.get_new_register<float_reg>();
		a->set_reg(temp);
		Ics_Opd *r3 = new Register_Addr_Opd(temp);
		Icode_Stmt * i = new Move_IC_Stmt(uminus_d,r2,r3);
		a->append_ics(*i);
	}
	r.get_reg()->reset_use_for_expr_result();
	return *a;
}

Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
}

//////////////////////////////////////////////////////////////////////

// Code_For_Ast & Relational_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
// 	Code_For_Ast x;
// 	return x;
// }

Code_For_Ast & Relational_Expr_Ast::compile(){
	Code_For_Ast l = lhs_condition->compile();
	Code_For_Ast r = rhs_condition->compile();

	list<Icode_Stmt *> x = l.get_icode_list();
	list<Icode_Stmt *> y = r.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
		a->append_ics(*(*it));
	}
	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r1 = new Register_Addr_Opd(l.get_reg());
	Ics_Opd *r2 = new Register_Addr_Opd(r.get_reg());
	Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
	a->set_reg(temp);
	Ics_Opd *r3 = new Register_Addr_Opd(temp);
	Icode_Stmt * i;
	if(node_data_type == int_data_type){
		if(rel_op == less_equalto) 
			i = new Compute_IC_Stmt(sle,r1,r2,r3);
		else if(rel_op == less_than)
			i = new Compute_IC_Stmt(slt,r1,r2,r3);
		else if(rel_op == greater_than)
			i = new Compute_IC_Stmt(sgt,r1,r2,r3);
		else if(rel_op == greater_equalto)
			i = new Compute_IC_Stmt(sge,r1,r2,r3);
		else if(rel_op == equalto)
			i = new Compute_IC_Stmt(seq,r1,r2,r3);
		else
			i = new Compute_IC_Stmt(sne,r1,r2,r3);
	}
	else{
		if(rel_op == less_equalto) 
			i = new Move_IC_Stmt(sle_d,r2,r1);
		else if(rel_op == less_than)
			i = new Move_IC_Stmt(slt_d,r2,r1);
		else if(rel_op == greater_than)
			i = new Move_IC_Stmt(sgt_d,r2,r1);
		else if(rel_op == greater_equalto)
			i = new Move_IC_Stmt(sge_d,r2,r1);
		else if(rel_op == equalto)
			i = new Move_IC_Stmt(seq_d,r2,r1);
		else
			i = new Move_IC_Stmt(sne_d,r2,r1);
	}
	a->append_ics(*i);
	l.get_reg()->reset_use_for_expr_result();
	r.get_reg()->reset_use_for_expr_result();
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Conditional_Expression_Ast::compile(){
	Code_For_Ast c = cond->compile();
	list<Icode_Stmt *> z = c.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = z.begin();it!=z.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r1 = new Register_Addr_Opd(c.get_reg());
	string label1 = get_new_label();
	string label2 = get_new_label();

	Icode_Stmt * i1;
	if(cond->get_data_type()==int_data_type)
		i1 = new Control_Flow_IC_Stmt(beq,r1,NULL,label1);
	else{
		Icode_Stmt * check = a->get_icode_list().back();
		if(check->get_op().get_op()==sgt_d || check->get_op().get_op()==sge_d || check->get_op().get_op()==sne_d)
			i1 = new Label_IC_Stmt(bc1t, label1);
		else
			i1 = new Label_IC_Stmt(bc1f, label1);
	}
	a->append_ics(*i1);
	c.get_reg()->reset_use_for_expr_result();

	Code_For_Ast l = lhs->compile();
	Code_For_Ast r = rhs->compile();

	list<Icode_Stmt *> x = l.get_icode_list();
	list<Icode_Stmt *> y = r.get_icode_list();

	for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
		a->append_ics(*(*it));
	}

	Register_Descriptor * temp;
	if(lhs->get_data_type() == int_data_type)
		temp = machine_desc_object.get_new_register<int_reg>();
	else
		temp = machine_desc_object.get_new_register<float_reg>();
	a->set_reg(temp);

	r1 = new Register_Addr_Opd(l.get_reg());
	Ics_Opd *r2 = new Register_Addr_Opd(machine_desc_object.spim_register_table[zero]);
	Ics_Opd *r3 = new Register_Addr_Opd(temp);

	Icode_Stmt * i2 = new Compute_IC_Stmt(or_t,r1,r2,r3);
	a->append_ics(*i2);
	l.get_reg()->reset_use_for_expr_result();

	Icode_Stmt * i3 = new Label_IC_Stmt(j,label2);
	a->append_ics(*i3);
	Icode_Stmt * i4 = new Label_IC_Stmt(label,label1);
	a->append_ics(*i4);

	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	r1 = new Register_Addr_Opd(r.get_reg());

	Icode_Stmt * i5 = new Compute_IC_Stmt(or_t,r1,r2,r3);
	a->append_ics(*i5);
	Icode_Stmt * i6 = new Label_IC_Stmt(label,label2);
	a->append_ics(*i6);
	
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Logical_Expr_Ast::compile(){
	Code_For_Ast * a = new Code_For_Ast();
	Ics_Opd *r1;
	Code_For_Ast l;
	if(bool_op != _logical_not){
		l = lhs_op->compile();
		list<Icode_Stmt *> x = l.get_icode_list();
		for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
			a->append_ics(*(*it));
		}
		r1 = new Register_Addr_Opd(l.get_reg());
	}
	Code_For_Ast r = rhs_op->compile();
		
	list<Icode_Stmt *> y = r.get_icode_list();

	for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
		a->append_ics(*(*it));
	}
	Ics_Opd *r2 = new Register_Addr_Opd(r.get_reg());
	Register_Descriptor * temp = machine_desc_object.get_new_register<int_reg>();
	a->set_reg(temp);
	Ics_Opd *r3 = new Register_Addr_Opd(temp);
	Icode_Stmt * i;
	if(bool_op == _logical_and) 
		i = new Compute_IC_Stmt(and_t,r1,r2,r3);
	else if(bool_op == _logical_or)
		i = new Compute_IC_Stmt(or_t,r1,r2,r3);
	else
		i = new Move_IC_Stmt(not_t,r2,r3);
	a->append_ics(*i);
	
	if(bool_op != _logical_not)
		l.get_reg()->reset_use_for_expr_result();
	r.get_reg()->reset_use_for_expr_result();
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Selection_Statement_Ast::compile(){
	string label1 = get_new_label();
	Code_For_Ast c = cond->compile();
	list<Icode_Stmt *> z = c.get_icode_list();
	Code_For_Ast * a = new Code_For_Ast();

	for(list<Icode_Stmt *>::iterator it = z.begin();it!=z.end();it++){
		a->append_ics(*(*it));
	}

	Ics_Opd *r1 = new Register_Addr_Opd(c.get_reg());
	Icode_Stmt * i1;
	if(cond->get_data_type()==int_data_type)
		i1 = new Control_Flow_IC_Stmt(beq,r1,NULL,label1);
	else{
		Icode_Stmt * check = a->get_icode_list().back();
		if(check->get_op().get_op()==sgt_d || check->get_op().get_op()==sge_d || check->get_op().get_op()==sne_d)
			i1 = new Label_IC_Stmt(bc1t, label1);
		else
			i1 = new Label_IC_Stmt(bc1f, label1);
	}
	a->append_ics(*i1);
	c.get_reg()->reset_use_for_expr_result();
	a->set_reg(NULL);

	if(else_part!=NULL){
		string label2 = get_new_label();

		Code_For_Ast l = then_part->compile();
		Code_For_Ast r = else_part->compile();

		list<Icode_Stmt *> x = l.get_icode_list();
		list<Icode_Stmt *> y = r.get_icode_list();

		for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
			a->append_ics(*(*it));
		}

		Icode_Stmt * i3 = new Label_IC_Stmt(j,label2);
		a->append_ics(*i3);
		Icode_Stmt * i4 = new Label_IC_Stmt(label,label1);
		a->append_ics(*i4);

		for(list<Icode_Stmt *>::iterator it = y.begin();it!=y.end();it++){
			a->append_ics(*(*it));
		}
		Icode_Stmt * i6 = new Label_IC_Stmt(label,label2);
		a->append_ics(*i6);
	}
	else{
		Code_For_Ast l = then_part->compile();
		list<Icode_Stmt *> x = l.get_icode_list();
		for(list<Icode_Stmt *>::iterator it = x.begin();it!=x.end();it++){
			a->append_ics(*(*it));
		}
		Icode_Stmt * i4 = new Label_IC_Stmt(label,label1);
		a->append_ics(*i4);
	}
	
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Iteration_Statement_Ast::compile(){
	string label1 = get_new_label();
	string label2 = get_new_label();

	Code_For_Ast * a = new Code_For_Ast();
	if(!is_do_form){
		Icode_Stmt * ij = new Label_IC_Stmt(j, label2);
		a->append_ics(*ij);
	}
	Icode_Stmt * i1 = new Label_IC_Stmt(label, label1);
	a->append_ics(*i1);

	Code_For_Ast b = body->compile();
	list<Icode_Stmt *> z = b.get_icode_list();

	for(list<Icode_Stmt *>::iterator it = z.begin();it!=z.end();it++){
		a->append_ics(*(*it));
	}

	Icode_Stmt * i2 = new Label_IC_Stmt(label, label2);
	a->append_ics(*i2);

	Code_For_Ast c = cond->compile();
	list<Icode_Stmt *> z1 = c.get_icode_list();

	for(list<Icode_Stmt *>::iterator it = z1.begin();it!=z1.end();it++){
		a->append_ics(*(*it));
	}

	Ics_Opd *r1 = new Register_Addr_Opd(c.get_reg());

	Icode_Stmt * i3;
	if(cond->get_data_type()==int_data_type)
		i3 = new Control_Flow_IC_Stmt(bne,r1,NULL,label1);
	else{
		Icode_Stmt * check = a->get_icode_list().back();
		if(check->get_op().get_op()==sgt_d || check->get_op().get_op()==sge_d || check->get_op().get_op()==sne_d)
			i3 = new Label_IC_Stmt(bc1f, label1);
		else
			i3 = new Label_IC_Stmt(bc1t, label1);
	}
	a->append_ics(*i3);
	c.get_reg()->reset_use_for_expr_result();

	a->set_reg(NULL);
	
	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Sequence_Ast::compile(){
	for(list<Ast*>::iterator it = statement_list.begin(); it != statement_list.end(); it++){
		Code_For_Ast x1 = (*it)->compile();
		if (x1.get_reg() != NULL)
			x1.get_reg()->reset_use_for_expr_result();
		list<Icode_Stmt *> x = x1.get_icode_list();
		for(list<Icode_Stmt *>::iterator it2 = x.begin(); it2!=x.end(); it2++){
			sa_icode_list.push_back(*it2);
		}
	}
	Code_For_Ast* t = new Code_For_Ast(sa_icode_list, NULL);
	return *t;
}

void Sequence_Ast::print_assembly(ostream & file_buffer){
	for (list<Icode_Stmt*>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); it++){
		(*it)->print_assembly(file_buffer);
	}
}

void Sequence_Ast::print_icode(ostream & file_buffer){
	for (list<Icode_Stmt*>::iterator it = sa_icode_list.begin(); it != sa_icode_list.end(); it++){
		(*it)->print_icode(file_buffer);
	}
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Print_Ast::compile(){
	Code_For_Ast * a = new Code_For_Ast();
	a->set_reg(NULL);
	Ics_Opd *r3 = new Mem_Addr_Opd(var->get_symbol_entry());
	if(var->get_data_type() == int_data_type){
		Ics_Opd *r1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]);
		Ics_Opd *r2 = new Const_Opd<int>(1);
		Icode_Stmt * i1 = new Move_IC_Stmt(imm_load, r2, r1);
		a->append_ics(*i1);
		r1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[a0]);
		Icode_Stmt * i = new Move_IC_Stmt(load,r3,r1);
		a->append_ics(*i);
	}
	else{
		Ics_Opd *r1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[v0]);
		Ics_Opd *r2 = new Const_Opd<int>(3);
		Icode_Stmt * i1 = new Move_IC_Stmt(imm_load, r2, r1);
		a->append_ics(*i1);
		r1 = new Register_Addr_Opd(machine_desc_object.spim_register_table[f12]);
		Icode_Stmt * i = new Move_IC_Stmt(load_d,r3,r1);
		a->append_ics(*i);
	}
	Icode_Stmt * i2 = new Print_IC_Stmt();
	a->append_ics(*i2);

	return *a;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Return_Ast::compile() {
	list <Icode_Stmt*> icl;
	if (return_value != NULL){
		Code_For_Ast c = return_value->compile();
		Register_Descriptor *reg = c.get_reg();
		for(list<Icode_Stmt *>::iterator i2 = c.get_icode_list().begin(); i2 != c.get_icode_list().end(); i2++){
			icl.push_back(*i2);
		}
		Ics_Opd *dst;
		Ics_Opd *src = new Register_Addr_Opd(reg);
		Icode_Stmt *ic_stmt;
		if(return_value->get_data_type() == int_data_type){
			dst = new Register_Addr_Opd(machine_desc_object.spim_register_table[v1]);
			ic_stmt = new Move_IC_Stmt(mov, src, dst);
		}
		else if(return_value->get_data_type() == double_data_type){
			dst = new Register_Addr_Opd(machine_desc_object.spim_register_table[f0]);
			ic_stmt = new Move_IC_Stmt(move_d, src, dst);
		}
		reg->reset_use_for_expr_result();
		icl.push_back(ic_stmt);
	}
	Icode_Stmt *ic_stmt = new Label_IC_Stmt(ret_inst, proc_name);
	icl.push_back(ic_stmt);
	Code_For_Ast* t = new Code_For_Ast(icl, NULL);
	return *t;
}

//////////////////////////////////////////////////////////////////////

Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra) {}

Code_For_Ast & Call_Ast::compile(){
	list<Icode_Stmt*> icl = list<Icode_Stmt*>();
	Procedure* p = program_object.get_procedure_prototype(procedure_name);
	Symbol_Table s = p->get_formal_param_list();
	list<Symbol_Table_Entry*> l2 = s.get_table();
	Symbol_Table sssss;
	list<Symbol_Table_Entry*> l;
	for (list<Symbol_Table_Entry*>::iterator i7 = l2.begin(); i7 != l2.end(); i7++){
		string nnn((*i7)->get_variable_name());
		Symbol_Table_Entry* sss = new Symbol_Table_Entry(nnn, (*i7)->get_data_type(), (*i7)->get_lineno(), sp_ref);
		sssss.push_symbol(sss);
	}
	sssss.set_table_scope(local);
	sssss.assign_offsets();
	l = sssss.get_table();
	list<Symbol_Table_Entry*>::iterator i3 = l.begin();

	for(list<Ast*>::iterator i1 = --actual_param_list.end(); i1 != --actual_param_list.begin(); i1--){
		Code_For_Ast c = (*i1)->compile();
		Register_Descriptor* r = c.get_reg();
		list<Icode_Stmt*> l2 = c.get_icode_list();
		for (list<Icode_Stmt*>::iterator i2 = l2.begin(); i2 != l2.end(); i2++){
			icl.push_back(*i2);
		}
		Ics_Opd * dst = new Mem_Addr_Opd(**i3);
		Ics_Opd* src = new Register_Addr_Opd(r);
		i3++;
		if((*i1)->get_data_type() == int_data_type){
			Icode_Stmt *ic_stmt = new Move_IC_Stmt(store, dst, src);
			icl.push_back(ic_stmt);
		}
		else if ((*i1)->get_data_type() == double_data_type){
			Icode_Stmt *ic_stmt = new Move_IC_Stmt(store_d, dst, src);
			icl.push_back(ic_stmt);
		}
		r->reset_use_for_expr_result();
	}
	
	Icode_Stmt *ic_stmt = new Label_IC_Stmt(jal, procedure_name);
	icl.push_back(ic_stmt);

	Register_Descriptor* reg2;
	if(node_data_type == int_data_type){
		return_value_reg = machine_desc_object.spim_register_table[v1];
		reg2 = machine_desc_object.get_new_register<int_reg>();
		Ics_Opd *src = new Register_Addr_Opd(return_value_reg);
		Ics_Opd *dst= new Register_Addr_Opd(reg2);
		Icode_Stmt *ic_stmt = new Move_IC_Stmt(mov, src, dst);
		icl.push_back(ic_stmt);
	}
	else if(node_data_type == double_data_type){
		return_value_reg = machine_desc_object.spim_register_table[f0];
		reg2 = machine_desc_object.get_new_register<float_reg>();
		Ics_Opd *src = new Register_Addr_Opd(return_value_reg);
		Ics_Opd *dst = new Register_Addr_Opd(reg2);
		Icode_Stmt *ic_stmt = new Move_IC_Stmt(mov, src, dst);
		icl.push_back(ic_stmt);
	}

	return *(new Code_For_Ast(icl, reg2));
}