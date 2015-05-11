#define MAX_MEM 1024
#define MAX_INS 1024
typedef long word;


char MEM[MAX_MEM] = { NULL };
word PC = 0;
word HI = 0;
word LO = 0;
word EXP = 0; //Òì³£´úÂë

#include "reg.h"
#include "op.h"
#include "op0.h"
#include "op1.h"
#include "opx.h"
#include "op1c.h"




class MIPS {
	typedef OP* (*make)(string s);
	static map<string, make> ops;

	static OP* makeOP(string s) { return parse(s); }
	static OP* parse(string s) {
		if (s == "") {
			cout << "NULL Input !" << endl;
			return NULL;
		}
		else if (s[0] >= 'a' && s[0] <= 'z')
			return parseAssembly(s);
		else if (s[0] == '0' || s[0] == '1')
			return parseCode(s);
		else {
			cout << "Wrong Input !" << endl;
			return NULL;
		}
	}
	static OP* parseAssembly(string s) {
		int n = s.find(' ');
		string op = s.substr(0, n);
		map<string, make>::iterator it = ops.find(op);
		if (it != ops.end()) {
			make fp = (*it).second;
			return (*fp)(s);
		}
		else {
			cout << "Wrong Assembly !" << endl;
			return NULL;
		}
	}
	static OP* parseCode(string s) {
		if (s.size() != 32) {
			cout << "Wrong Code !" << endl;
			return NULL;
		}
		int i;
		for (i = 0; i < 32; ++i) {
			if (s[i] != '0' && s[i] != '1')
				break;
		}
		if (i < 32) {
			cout << "Wrong Code !" << endl;
			return NULL;
		}
		word w = stow(s);
		word op = (w >> 26) & 0x3F;
		if (op == 0)
			return parseCode0(w);
		else if (op == 1)
			return parseCode1(w);
		else
			return parseCodeX(w);
	}
	static OP* parseCode0(word w) {
		word funct = w & 0x3F;
		switch (funct) {
		case 0x00: return SLL::makeOP(w); break;
		case 0x02: return SRL::makeOP(w); break;
		case 0x03: return SRA::makeOP(w); break;
		case 0x04: return SLLV::makeOP(w); break;
		case 0x06: return SRLV::makeOP(w); break;
		case 0x07: return SRAV::makeOP(w); break;
		case 0x08: return JR::makeOP(w); break;
		case 0x09: return JALR::makeOP(w); break;
		case 0x0A: return MOVZ::makeOP(w); break;
		case 0x0B: return MOVN::makeOP(w); break;
		case 0x10: return MFHI::makeOP(w); break;
		case 0x11: return MTHI::makeOP(w); break;
		case 0x12: return MFLO::makeOP(w); break;
		case 0x13: return MTLO::makeOP(w); break;
		case 0x18: return MULT::makeOP(w); break;
		case 0x19: return MULTU::makeOP(w); break;
		case 0x1A: return DIV::makeOP(w); break;
		case 0x1B: return DIVU::makeOP(w); break;
		case 0x20: return ADD::makeOP(w); break;
		case 0x21: return ADDU::makeOP(w); break;
		case 0x22: return SUB::makeOP(w); break;
		case 0x23: return SUBU::makeOP(w); break;
		case 0x24: return AND::makeOP(w); break;
		case 0x25: return OR::makeOP(w); break;
		case 0x26: return XOR::makeOP(w); break;
		case 0x27: return NOR::makeOP(w); break;
		case 0x2A: return SLT::makeOP(w); break;
		case 0x2B: return SLTU::makeOP(w); break;
		case 0x30: return TGE::makeOP(w); break;
		case 0x31: return TGEU::makeOP(w); break;
		case 0x32: return TLT::makeOP(w); break;
		case 0x33: return TLTU::makeOP(w); break;
		case 0x34: return TEQ::makeOP(w); break;
		case 0x36: return TNE::makeOP(w); break;
		default: {
					 cout << "Wrong Code0 FUNCT !" << endl;
					 return NULL;
		}
		}
	}
	static OP* parseCode1(word w) {
		word rt = (w >> 16) & 0x1F;
		switch (rt) {
		case 0x00: return BLTZ::makeOP(w); break;
		case 0x01: return BGEZ::makeOP(w); break;
		case 0x08: return TGEI::makeOP(w); break;
		case 0x09: return TGEIU::makeOP(w); break;
		case 0x0A: return TLTI::makeOP(w); break;
		case 0x0B: return TLTIU::makeOP(w); break;
		case 0x0C: return TEQI::makeOP(w); break;
		case 0x0E: return TNEI::makeOP(w); break;
		case 0x10: return BLTZAL::makeOP(w); break;
		case 0x11: return BGEZAL::makeOP(w); break;
		default: {
					 cout << "Wrong Code1 RT !" << endl;
					 return NULL;
		}
		}
	}
	static OP* parseCodeX(word w) {
		word op = (w >> 26) & 0x3F;
		switch (op) {
		case 0x02: return J::makeOP(w); break;
		case 0x03: return JAL::makeOP(w); break;
		case 0x04: return BEQ::makeOP(w); break;
		case 0x05: return BNE::makeOP(w); break;
		case 0x06: return BLEZ::makeOP(w); break;
		case 0x07: return BGTZ::makeOP(w); break;
		case 0x08: return ADDI::makeOP(w); break;
		case 0x09: return ADDIU::makeOP(w); break;
		case 0x0A: return SLTI::makeOP(w); break;
		case 0x0B: return SLTIU::makeOP(w); break;
		case 0x0C: return ANDI::makeOP(w); break;
		case 0x0D: return ORI::makeOP(w); break;
		case 0x0E: return XORI::makeOP(w); break;
		case 0x0F: return LUI::makeOP(w); break;
		case 0x1C: return parseCode1C(w); break;
		case 0x20: return LB::makeOP(w); break;
		case 0x21: return LH::makeOP(w); break;
		case 0x22: return LWL::makeOP(w); break;
		case 0x23: return LW::makeOP(w); break;
		case 0x24: return LBU::makeOP(w); break;
		case 0x25: return LHU::makeOP(w); break;
		case 0x26: return LWR::makeOP(w); break;
		case 0x28: return SB::makeOP(w); break;
		case 0x29: return SH::makeOP(w); break;
		case 0x2A: return SWL::makeOP(w); break;
		case 0x2B: return SW::makeOP(w); break;
		case 0x2E: return SWR::makeOP(w); break;
		case 0x30: return LL::makeOP(w); break;
		case 0x38: return SC::makeOP(w); break;
		default: {
					 cout << "Wrong CodeX OP !" << endl;
					 return NULL;
		}
		}
	}
	static OP* parseCode1C(word w) {
		word funct = w & 0x3F;
		switch (funct) {
		case 0x00: return MADD::makeOP(w); break;
		case 0x01: return MADDU::makeOP(w); break;
		case 0x02: return MUL::makeOP(w); break;
		case 0x04: return MSUB::makeOP(w); break;
		case 0x05: return MSUBU::makeOP(w); break;
		case 0x20: return CLZ::makeOP(w); break;
		case 0x21: return CLO::makeOP(w); break;
		default: {
					 cout << "Wrong Code1C FUNCT !" << endl;
					 return NULL;
		}
		}
	}
	friend class screen;
	/*friend istream& operator>>(istream& is, OP*& op);*/
};

map<string, MIPS::make> MIPS::ops = {
	{ "sll", SLL::makeOP },
	{ "srl", SRL::makeOP },
	{ "sra", SRA::makeOP },
	{ "sllv", SLLV::makeOP },
	{ "srlv", SRLV::makeOP },
	{ "srav", SRAV::makeOP },
	{ "jr", JR::makeOP },
	{ "jalr", JALR::makeOP },
	{ "movz", MOVZ::makeOP },
	{ "movn", MOVN::makeOP },
	{ "mfhi", MFHI::makeOP },
	{ "mthi", MTHI::makeOP },
	{ "mflo", MFLO::makeOP },
	{ "mtlo", MTLO::makeOP },
	{ "mult", MULT::makeOP },
	{ "multu", MULTU::makeOP },
	{ "div", DIV::makeOP },
	{ "divu", DIVU::makeOP },
	{ "add", ADD::makeOP },
	{ "addu", ADDU::makeOP },
	{ "sub", SUB::makeOP },
	{ "subu", SUBU::makeOP },
	{ "and", AND::makeOP },
	{ "or", OR::makeOP },
	{ "xor", XOR::makeOP },
	{ "nor", NOR::makeOP },
	{ "slt", SLT::makeOP },
	{ "sltu", SLTU::makeOP },
	{ "tge", TGE::makeOP },
	{ "tgeu", TGEU::makeOP },
	{ "tlt", TLT::makeOP },
	{ "tltu", TLTU::makeOP },
	{ "teq", TEQ::makeOP },
	{ "tne", TNE::makeOP },
	{ "bltz", BLTZ::makeOP },
	{ "bgez", BGEZ::makeOP },
	{ "tgei", TGEI::makeOP },
	{ "tgeiu", TGEIU::makeOP },
	{ "tlti", TLTI::makeOP },
	{ "tltiu", TLTIU::makeOP },
	{ "teqi", TEQI::makeOP },
	{ "tnei", TNEI::makeOP },
	{ "bltzal", BLTZAL::makeOP },
	{ "bgezal", BGEZAL::makeOP },
	{ "j", J::makeOP },
	{ "jal", JAL::makeOP },
	{ "beq", BEQ::makeOP },
	{ "bne", BNE::makeOP },
	{ "blez", BLEZ::makeOP },
	{ "bgez", BGEZ::makeOP },
	{ "addi", ADDI::makeOP },
	{ "addiu", ADDIU::makeOP },
	{ "slti", SLTI::makeOP },
	{ "sltiu", SLTIU::makeOP },
	{ "andi", ANDI::makeOP },
	{ "ori", ORI::makeOP },
	{ "xori", XORI::makeOP },
	{ "lui", LUI::makeOP },
	{ "madd", MADD::makeOP },
	{ "maddu", MADDU::makeOP },
	{ "mul", MUL::makeOP },
	{ "msub", MSUB::makeOP },
	{ "msubu", MSUBU::makeOP },
	{ "clz", CLZ::makeOP },
	{ "clo", CLO::makeOP },
	{ "lb", LB::makeOP },
	{ "lh", LH::makeOP },
	{ "lwl", LWL::makeOP },
	{ "lw", LW::makeOP },
	{ "lbu", LBU::makeOP },
	{ "lhu", LHU::makeOP },
	{ "lwr", LWR::makeOP },
	{ "sb", SB::makeOP },
	{ "sh", SH::makeOP },
	{ "swl", SWL::makeOP },
	{ "sw", SW::makeOP },
	{ "swr", SWR::makeOP },
	{ "ll", LL::makeOP },
	{ "sc", SC::makeOP }
};
/*
istream& operator>>(istream& is, OP*& op) {
	string s;
	getline(cin, s);
	op = MIPS::parse(s);
	return is;
}

ostream& operator<<(ostream& os, OP*& op) {
	if (op != NULL) {
		os << "Assembly Language :" << op->toAssembly() << endl;
		os << "Machine Code :" << hex << wtos(op->toCode()) << endl;
	}
	return os;
}
*/
