#include<iostream> 
#include <sstream> 
#include <string>

template class Const_Opd<double>;
template class Const_Opd<int>;
using namespace std;

Instruction_Descriptor::Instruction_Descriptor(Tgt_Op op, string name_, string mnn, string ics, Icode_Format icf, Assembly_Format af){
    inst_op = op;
    name = name_;
    mnemonic = mnn;
    ic_symbol = ics;
    ic_format = icf;
    assem_format = af;
}

Instruction_Descriptor::Instruction_Descriptor() {}

Tgt_Op Instruction_Descriptor::get_op(){
    return inst_op;
}

string Instruction_Descriptor::get_name(){
    return name;
}

string Instruction_Descriptor::get_mnemonic(){
    return mnemonic;
}

string Instruction_Descriptor::get_ic_symbol(){
    return ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format(){
    return ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format(){
     return assem_format;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){
    file_buffer.write(name.c_str(), name.size());
}

////////////////////////////////////////////////////////////////////////////

Register_Descriptor* Ics_Opd::get_reg(){
    Register_Descriptor* reg = new Register_Descriptor();
    return reg;
}

////////////////////////////////////////////////////////////////////////////

Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
    symbol_entry = &se;
}

Mem_Addr_Opd& Mem_Addr_Opd::operator=(const Mem_Addr_Opd & rhs){
    symbol_entry = rhs.symbol_entry;
    return *this;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){
    string name = symbol_entry->get_variable_name();
    file_buffer.write(name.c_str(), name.size());
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){
    Table_Scope sc =  symbol_entry->get_symbol_scope();
    if (sc == global){
        string name = symbol_entry->get_variable_name();
        file_buffer.write(name.c_str(), name.size());
    }
    else{
        int offset = symbol_entry->get_start_offset();
        ostringstream str1; 
        str1 << offset;
        string offstr = str1.str();
        string suff("($fp)");
        Offset_With_Ref_To rrrr = symbol_entry->get_ref_offset();
        if (rrrr == sp_ref){
            suff = string("($sp)");
        }
        string tmp = offstr + suff;
        file_buffer.write(tmp.c_str(), tmp.size());
    }
}

////////////////////////////////////////////////////////////////////////////
Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
    // register_description = new Register_Descriptor()
    register_description = rd;
}

Register_Descriptor* Register_Addr_Opd::get_reg(){
    return register_description;
}

Register_Addr_Opd & Register_Addr_Opd::operator=(const Register_Addr_Opd & rhs){
    register_description = rhs.register_description;
    return *this;
}

void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){
    string dol("$");
    string fin = register_description->get_name();
    file_buffer.write(fin.c_str(), fin.size());
}

void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){
    string dol("$");
    string fin = dol + register_description->get_name();
    file_buffer.write(fin.c_str(), fin.size());
}


////////////////////////////////////////////////////////////////////////////
template<class T>
Const_Opd<T>::Const_Opd (T num_){
    num = num_;
}

template<class T>
Const_Opd<T>& Const_Opd<T>::operator=(const Const_Opd & rhs){
    num = rhs.num;
    return *this;
}

template<class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer){
    ostringstream str1; 
    str1 << num;
    string offstr = str1.str();
    file_buffer.write(offstr.c_str(), offstr.size());
}

template<class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer){
    ostringstream str1; 
    str1 << num;
    string offstr = str1.str();
    file_buffer.write(offstr.c_str(), offstr.size());
}

////////////////////////////////////////////////////////////////////////////

Instruction_Descriptor & Icode_Stmt::get_op(){
    return op_desc;
}

Ics_Opd* Icode_Stmt::get_opd1(){
    return NULL;
}

Ics_Opd* Icode_Stmt::get_opd2(){
    return NULL;
}

Ics_Opd* Icode_Stmt::get_result(){
    return NULL;
}

void Icode_Stmt::set_opd1(Ics_Opd * io) {}
void Icode_Stmt::set_opd2(Ics_Opd * io) {}
void Icode_Stmt::set_result(Ics_Opd * io) {}

////////////////////////////////////////////////////////////////////////////

Print_IC_Stmt::Print_IC_Stmt() {}

void Print_IC_Stmt::print_icode(ostream & file_buffer){
    string fin("\tprint\n");
    file_buffer.write(fin.c_str(), fin.size());
}

void Print_IC_Stmt::print_assembly(ostream & file_buffer){
    string fin("\tsyscall\n");
    file_buffer.write(fin.c_str(), fin.size());
}

////////////////////////////////////////////////////////////////////////////
Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1_, Ics_Opd * result_){
    opd1 = opd1_;
    result = result_;
    op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
}

Move_IC_Stmt& Move_IC_Stmt::operator=(const Move_IC_Stmt& rhs){
    opd1 = rhs.opd1;
    result = rhs.result;
    op_desc = rhs.op_desc;
    return *this;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
    return op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1(){
    return opd1;
}

void Move_IC_Stmt::set_opd1(Ics_Opd * io){
    opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result(){
    return result;
}

void Move_IC_Stmt::set_result(Ics_Opd * io){
    result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer){
    string big(":    \t");
    string arr(" <- ");
    string space(" ");
    string com_sp(" , ");
    string tab("\t");
    string topr =  tab + op_desc.get_name();
    file_buffer.write(topr.c_str(), topr.size());
    file_buffer.write(big.c_str(), big.size());

    if (op_desc.get_op() != store && op_desc.get_op() != store_d){
        result->print_ics_opd(file_buffer);
    }
    else{
        opd1->print_ics_opd(file_buffer);
    }

    file_buffer.write(arr.c_str(), arr.size());
    
    if (op_desc.get_op() == store || op_desc.get_op() == store_d){
        result->print_ics_opd(file_buffer);
    }
    else{
        opd1->print_ics_opd(file_buffer);
    }
    
    string n("\n");
    file_buffer.write(n.c_str(), n.size());
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer){
    string space(" ");
    string com_sp(", ");
    string tab("\t");
    string topr = tab + op_desc.get_mnemonic();
    file_buffer.write(topr.c_str(), topr.size());
    file_buffer.write(space.c_str(), space.size());
    result->print_asm_opd(file_buffer);
    file_buffer.write(com_sp.c_str(), com_sp.size());
    opd1->print_asm_opd(file_buffer);
    string n("\n");
    file_buffer.write(n.c_str(), n.size());
}

////////////////////////////////////////////////////////////////////////////
Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1_, Ics_Opd * opd2_, Ics_Opd * result_){
    opd1 = opd1_;
    opd2 = opd2_;
    result = result_;
    op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
}

Compute_IC_Stmt& Compute_IC_Stmt::operator=(const Compute_IC_Stmt& rhs){
    opd1 = rhs.opd1;
    opd2 = rhs.opd2;
    result = rhs.result;
    op_desc = rhs.op_desc;
    return *this;
}

Ics_Opd * Compute_IC_Stmt::get_opd1(){
    return opd1;
}

void Compute_IC_Stmt::set_opd1(Ics_Opd * io){
    opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2(){
    return opd2;
}

void Compute_IC_Stmt::set_opd2(Ics_Opd * io){
    opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result(){
    return result;
}

void Compute_IC_Stmt::set_result(Ics_Opd * io){
    result = io;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
    string big(":    \t");
    string arr(" <- ");
    string space(" ");
    string com_sp(" , ");
    string n("\n");
    string tab("\t");
    string topr = tab + op_desc.get_name();

    file_buffer.write(topr.c_str(), topr.size());
    file_buffer.write(big.c_str(), big.size());
    result->print_ics_opd(file_buffer);
    file_buffer.write(arr.c_str(), arr.size());
    opd1->print_ics_opd(file_buffer);
    file_buffer.write(com_sp.c_str(), com_sp.size());
    opd2->print_ics_opd(file_buffer);
    file_buffer.write(n.c_str(), n.size());
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
    string space(" ");
    string com_sp(", ");
    string n("\n");
    string tab("\t");
    string topr = tab + op_desc.get_mnemonic();

    file_buffer.write(topr.c_str(), topr.size());
    file_buffer.write(space.c_str(), space.size());
    result->print_asm_opd(file_buffer);
    file_buffer.write(com_sp.c_str(), com_sp.size());
    opd1->print_asm_opd(file_buffer);
    file_buffer.write(com_sp.c_str(), com_sp.size());
    opd2->print_asm_opd(file_buffer);
    file_buffer.write(n.c_str(), n.size());
}

////////////////////////////////////////////////////////////

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1, Ics_Opd * o2, string label, int size){
    opd1 = o1;
    opd2 = o2;
    offset = label;
	actual_param_size = size;
    op_desc = *(machine_desc_object.spim_instruction_table[op]);
}

Control_Flow_IC_Stmt & Control_Flow_IC_Stmt::operator=(const Control_Flow_IC_Stmt & rhs){
    opd1 = rhs.opd1;
    opd2 = rhs.opd2;
    offset = rhs.offset;
    op_desc = rhs.op_desc;
    return *this;
}

Instruction_Descriptor& Control_Flow_IC_Stmt::get_inst_op_of_ics(){
    return op_desc;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd1(){
    return opd1;
}

void Control_Flow_IC_Stmt::set_opd1(Ics_Opd * io){
    opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2(){
    return opd2;
}

void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io){
    opd2 = io;
}

string Control_Flow_IC_Stmt::get_Offset(){
    return offset;
}

void Control_Flow_IC_Stmt::set_Offset(string label_){
    offset = label_;
}

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer){
    string big(":    \t");
    string space(" ");
    string n("\n");
    string tab("\t");
    string ze(" , zero : goto ");
    string topr = tab + op_desc.get_name();

    file_buffer.write(topr.c_str(), topr.size());
    file_buffer.write(big.c_str(), big.size());
    opd1->print_ics_opd(file_buffer);
    // file_buffer.write(space.c_str(), space.size());
    file_buffer.write(ze.c_str(), ze.size());
    file_buffer.write(offset.c_str(), offset.size());
    file_buffer.write(n.c_str(), n.size());
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer){
    string space(" ");
    string n("\n");
    string tab("\t");
    string ze(", $zero, ");
    string topr = tab + op_desc.get_mnemonic();

    file_buffer.write(topr.c_str(), topr.size());
    file_buffer.write(space.c_str(), space.size());
    opd1->print_asm_opd(file_buffer);
    // file_buffer.write(space.c_str(), space.size());
    file_buffer.write(ze.c_str(), ze.size());
    file_buffer.write(offset.c_str(), offset.size());
    file_buffer.write(n.c_str(), n.size());
}

/////////////////////////////////////////////////////////////////

Tgt_Op label_op = label;

Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label_){
    op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);
    label = label_;
}

Instruction_Descriptor & Label_IC_Stmt::get_inst_op_of_ics(){
    return op_desc;
}

string Label_IC_Stmt::get_label(){
    return label;
}

void Label_IC_Stmt::set_label(string label_){
    label = label_;
}

void Label_IC_Stmt::print_icode(ostream & file_buffer){
    if (op_desc.get_op() == label_op){
        string n("\n");
        string c(":        ");
        file_buffer.write(n.c_str(), n.size());
        file_buffer.write(label.c_str(), label.size());
        file_buffer.write(c.c_str(), c.size());
        file_buffer.write(n.c_str(), n.size());
    }
	else if(op_desc.get_name() == "return"){
		file_buffer << "\t" << "return" << "\n";
	}
	else if(op_desc.get_name() == "jal"){
		file_buffer << "\t" << "call " << label << "\n";
	}
    else{
        string space(" ");
        string tab("\t");
        string n("\n");
        string nname = op_desc.get_name();
        string got("goto");
        if (nname == "jump"){
            nname = got;
        }
        string topr = tab + nname;

        file_buffer.write(topr.c_str(), topr.size());
        file_buffer.write(space.c_str(), space.size());
        file_buffer.write(label.c_str(), label.size());
        file_buffer.write(n.c_str(), n.size());
    }
}

void Label_IC_Stmt::print_assembly(ostream & file_buffer){
    if (op_desc.get_op() == label_op){
        string n("\n");
        string c(":        ");
        file_buffer.write(n.c_str(), n.size());
        file_buffer.write(label.c_str(), label.size());
        file_buffer.write(c.c_str(), c.size());
        file_buffer.write(n.c_str(), n.size());
    }
	else if(op_desc.get_name() == "return"){
		file_buffer << "\tj epilogue_" << label << "\n";
	}
	else if(op_desc.get_name() == "jal"){
        int size = 0;
        list<Symbol_Table_Entry *> ttt = program_object.get_procedure_prototype(label)->get_formal_param_list().get_table();
        for (list<Symbol_Table_Entry *>::iterator it = ttt.begin(); it != ttt.end(); it++){
          size++;
        }
		file_buffer << "\tsub $sp, $sp, " << size * 4 << "\n";
		file_buffer << "\t" << "jal " << label << "\n";
		file_buffer << "\tadd $sp, $sp, " << size * 4 << "\n";
	}
    else{
        string space(" ");
        string n("\n");
        string tab("\t");
        string topr = tab + op_desc.get_mnemonic();

        file_buffer.write(topr.c_str(), topr.size());
        file_buffer.write(space.c_str(), space.size());
        file_buffer.write(label.c_str(), label.size());
        file_buffer.write(n.c_str(), n.size());
    }
}

////////////////////////////////////////////////////////////////////
Code_For_Ast::Code_For_Ast() {}

Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg){
    ics_list = ic_l;
    result_register = reg;
}

void Code_For_Ast::append_ics(Icode_Stmt & ics){
    ics_list.push_back(&ics);
}

list<Icode_Stmt *>& Code_For_Ast::get_icode_list(){
    return ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg(){
    return result_register;
}

void Code_For_Ast::set_reg(Register_Descriptor * reg){
    result_register = reg;
}

Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){
    ics_list = rhs.ics_list;
    result_register = rhs.result_register;
    return *this;
}
////////////////////////////////////////////////////////////////////////