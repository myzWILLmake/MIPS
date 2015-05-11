
class Reg {
	string name;
	word ID;
	word val;
public:
	Reg(string nm, int id) : name(nm), ID(id) {val = 0;}

	friend class OPR;
	friend class OPRS;
	friend class OPRMDT;
	friend class OPI1;
	friend class OPI;
	friend class OPII;
	friend class OPILS;

	friend class SLL;
	friend class SRL;
	friend class SRA;
	friend class SLLV;
	friend class SRLV;
	friend class SRAV;
	friend class JR;
	friend class JALR;
	friend class MOVZ;
	friend class MOVN;
	friend class MFHI;
	friend class MTHI;
	friend class MFLO;
	friend class MTLO;
	friend class MULT;
	friend class MULTU;
	friend class DIV;
	friend class DIVU;
	friend class ADD;
	friend class ADDU;
	friend class SUB;
	friend class SUBU;
	friend class AND;
	friend class OR;
	friend class XOR;
	friend class NOR;
	friend class SLT;
	friend class SLTU;
	friend class TGE;
	friend class TGEU;
	friend class TLT;
	friend class TLTU;
	friend class TEQ;
	friend class TNE;
	friend class BLTZ;
	friend class BGEZ;
	friend class TGEI;
	friend class TGEIU;
	friend class TLTI;
	friend class TLTIU;
	friend class TEQI;
	friend class TNEI;
	friend class BLTZAL;
	friend class BGEZAL;
	friend class J;
	friend class JAL;
	friend class BEQ;
	friend class BNE;
	friend class BLEZ;
	friend class BGTZ;
	friend class ADDI;
	friend class ADDIU;
	friend class SLTI;
	friend class SLTIU;
	friend class ANDI;
	friend class ORI;
	friend class XORI;
	friend class LUI;
	friend class MADD;
	friend class MADDU;
	friend class MUL;
	friend class MSUB;
	friend class MSUBU;
	friend class CLZ;
	friend class CLO;
	friend class LB;
	friend class LH;
	friend class LWL;
	friend class LW;
	friend class LBU;
	friend class LHU;
	friend class LWR;
	friend class SB;
	friend class SH;
	friend class SWL;
	friend class SW;
	friend class SWR;
	friend class LL;
	friend class SC;

	friend class screen;
};

Reg* $zero = new Reg("$zero", 0);
Reg* $at = new Reg("$at", 1);
Reg* $v0 = new Reg("$v0", 2);
Reg* $v1 = new Reg("$v1", 3);
Reg* $a0 = new Reg("$a0", 4);
Reg* $a1 = new Reg("$a1", 5);
Reg* $a2 = new Reg("$a2", 6);
Reg* $a3 = new Reg("$a3", 7);
Reg* $t0 = new Reg("$t0", 8);
Reg* $t1 = new Reg("$t1", 9);
Reg* $t2 = new Reg("$t2", 10); 
Reg* $t3 = new Reg("$t3", 11);
Reg* $t4 = new Reg("$t4", 12); 
Reg* $t5 = new Reg("$t5", 13); 
Reg* $t6 = new Reg("$t6", 14); 
Reg* $t7 = new Reg("$t7", 15);
Reg* $s0 = new Reg("$s0", 16); 
Reg* $s1 = new Reg("$s1", 17); 
Reg* $s2 = new Reg("$s2", 18); 
Reg* $s3 = new Reg("$s3", 19);
Reg* $s4 = new Reg("$s4", 20); 
Reg* $s5 = new Reg("$s5", 21); 
Reg* $s6 = new Reg("$s6", 22); 
Reg* $s7 = new Reg("$s7", 23);
Reg* $t8 = new Reg("$t8", 24); 
Reg* $t9 = new Reg("$t9", 25); 
Reg* $k0 = new Reg("$k0", 26); 
Reg* $k1 = new Reg("$k1", 27);
Reg* $gp = new Reg("$gp", 28); 
Reg* $sp = new Reg("$sp", 29); 
Reg* $fp = new Reg("$fp", 30); 
Reg* $ra = new Reg("$ra", 31);

map<string, Reg*> regs = {
	{ "$zero", $zero },
	{ "$at", $at },
	{ "$v0", $v0 },
	{ "$v1", $v1 },
	{ "$a0", $a0 },
	{ "$a1", $a1 },
	{ "$a2", $a2 },
	{ "$a3", $a3 },
	{ "$t0", $t0 },
	{ "$t1", $t1 },
	{ "$t2", $t2 },
	{ "$t3", $t3 },
	{ "$t4", $t4 },
	{ "$t5", $t5 },
	{ "$t6", $t6 },
	{ "$t7", $t7 },
	{ "$s0", $s0 },
	{ "$s1", $s1 },
	{ "$s2", $s2 },
	{ "$s3", $s3 },
	{ "$s4", $s4 },
	{ "$s5", $s5 },
	{ "$s6", $s6 },
	{ "$s7", $s7 },
	{ "$t8", $t8 },
	{ "$t9", $t9 },
	{ "$k0", $k0 },
	{ "$k1", $k1 },
	{ "$gp", $gp },
	{ "$sp", $sp },
	{ "$fp", $fp },
	{ "$ra", $ra }
};

Reg* parseRegister(word reg) {
	switch (reg) {
	case 0: return $zero; break;
	case 1: return $at; break;
	case 2: return $v0; break;
	case 3: return $v1; break;
	case 4: return $a0; break;
	case 5: return $a1; break;
	case 6: return $a2; break;
	case 7: return $a3; break;
	case 8: return $t0; break;
	case 9: return $t1; break;
	case 10: return $t2; break;
	case 11: return $t3; break;
	case 12: return $t4; break;
	case 13: return $t5; break;
	case 14: return $t6; break;
	case 15: return $t7; break;
	case 16: return $s0; break;
	case 17: return $s1; break;
	case 18: return $s2; break;
	case 19: return $s3; break;
	case 20: return $s4; break;
	case 21: return $s5; break;
	case 22: return $s6; break;
	case 23: return $s7; break;
	case 24: return $t8; break;
	case 25: return $t9; break;
	case 26: return $k0; break;
	case 27: return $k1; break;
	case 28: return $gp; break;
	case 29: return $sp; break;
	case 30: return $fp; break;
	case 31: return $ra; break;
	}
}
