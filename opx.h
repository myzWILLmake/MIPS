
class OPI : public OP {
protected:
	Reg *rs, *rt;
	word imm;

	bool parseAssembly(string s) {
		int i = s.find(' ');
		int j = s.find(',');
		int k = s.rfind(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return false;
		}
		r = s.substr(j + 1, k - j - 1); 
		it = regs.find(r);
		if (it != regs.end())
			rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return false;
		}
		string w = s.substr(k + 1);
		imm = stol(w);
		return true;
	}
	void parseCode(word w) {
		word r = (w >> 21) & 0x1F;
		rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		rt = parseRegister(r);
		imm = w & 0xFFFF;
	}
	virtual string toAssembly() {
		return rs->name + "," + rt->name + "," + immtos(imm, 16);
	}
	virtual word toCode() {
		return (rs->ID << 21) | (rt->ID << 16) | imm;
	}
};

// addi/slti/andi/ori/xori
class OPII : public OP {
protected:
	Reg *rs, *rt;
	word imm;

	bool parseAssembly(string s) {
		int i = s.find(' ');
		int j = s.find(',');
		int k = s.rfind(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return false;
		}
		r = s.substr(j + 1, k - j - 1);
		it = regs.find(r);
		if (it != regs.end())
			rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return false;
		}
		string w = s.substr(k + 1);
		imm = stol(w) & 0xFFFF;
		return true;
	}
	void parseCode(word w) {
		word r = (w >> 21) & 0x1F;
		rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		rt = parseRegister(r);
		imm = w & 0xFFFF;
	}
	virtual string toAssembly() {
		return rt->name + "," + rs->name + "," + immtos(imm, 16);
	}
	virtual word toCode() {
		return (rs->ID << 21) | (rt->ID << 16) | imm;
	}
};

// lw*/lh*/lb*/sw*/sh/sb/ll/sc
class OPILS : public OP {
protected:
	Reg *rs, *rt;
	word imm;

	bool parseAssembly(string s) {
		int i = s.find(' ');
		int j = s.find(',');
		int k1 = s.find('(');
		int k2 = s.rfind(')');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return false;
		}
		string w = s.substr(j + 1, k1 - j - 1);
		imm = stol(w) & 0xFFFF;
		r = s.substr(k1 + 1, k2 - k1 - 1);
		it = regs.find(r);
		if (it != regs.end())
			rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return false;
		}
		return true;
	}
	void parseCode(word w) {
		word r = (w >> 21) & 0x1F;
		rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		rt = parseRegister(r);
		imm = w & 0xFFFF;
	}
	virtual string toAssembly() {
		return rt->name + "," + immtos(imm, 16) + "(" + rs->name + ")";
	}
	virtual word toCode() {
		return (rs->ID << 21) | (rt->ID << 16) | imm;
	}
};

class J : public OP {
	string sop = "j";
	word wop = 0x08000000;
	word imm;

	J() {}
public:
	virtual string toAssembly() {
		return sop + " " + immtos(imm, 26);
	}
	virtual word toCode() {
		return wop | imm;
	}
	virtual word runOP() {
		PC = (PC & 0xF0000000) | (imm << 2);
		return 0;
	}
	static OP* makeOP(string s) {
		J* j = new J();
		int i = s.find(' ');
		string w = s.substr(i + 1);
		j->imm = stol(w) & 0x3FFFFFF;
		return j;
	}
	static OP* makeOP(word w) {
		J* j = new J();
		j->imm = w & 0x3FFFFFF;
		return j;
	}
};

class JAL : public OP {
	string sop = "jal";
	word wop = 0x0C000000;
	word imm;

	JAL() {}
public:
	virtual string toAssembly() {
		return sop + " " + immtos(imm, 26);
	}
	virtual word toCode() {
		return wop | imm;
	}
	virtual word runOP() {
		$ra->val = PC + 4;
		PC = (PC & 0xF0000000) | (imm << 2);
		return 0;
	}
	static OP* makeOP(string s) {
		JAL* jal = new JAL();
		int i = s.find(' ');
		string w = s.substr(i + 1);
		jal->imm = stol(w) & 0x3FFFFFF;
		return jal;
	}
	static OP* makeOP(word w) {
		JAL* jal = new JAL();
		jal->imm = w & 0x3FFFFFF;
		return jal;
	}
};

class BEQ : public OPI {
	string sop = "beq";
	word wop = 0x10000000;

	BEQ() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI::toCode();
	}
	virtual word runOP() {
		PC += 4;
		if (rs->val == rt->val) PC += imm * 4;
		return 0;
	}
	static OP* makeOP(string s) {
		BEQ* beq = new BEQ();
		if (beq->parseAssembly(s))
			return beq;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		BEQ* beq = new BEQ();
		beq->parseCode(w);
		return beq;
	}
};

class BNE : public OPI {
	string sop = "bne";
	word wop = 0x14000000;

	BNE() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI::toCode();
	}
	virtual word runOP() {
		PC += 4;
		if (rs->val != rt->val) PC += imm * 4;
		return 0;
	}
	static OP* makeOP(string s) {
		BNE* bne = new BNE();
		if (bne->parseAssembly(s))
			return bne;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		BNE* bne = new BNE();
		bne->parseCode(w);
		return bne;
	}
};

class BLEZ : public OP {
	string sop = "blez";
	word wop = 0x18000000;
	Reg *rs;
	word imm;

	BLEZ() {}
public:
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + immtos(imm, 16);
	}
	virtual word toCode() {
		return wop | (rs->ID & 0x1F) | imm;
	}
	virtual word runOP() {
		PC += 4;
		if (rs->val <= 0) PC += imm * 4;
		return 0;
	}
	static OP* makeOP(string s) {
		BLEZ* blez = new BLEZ();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			blez->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
 		}
		string w = s.substr(j + 1);
		blez->imm = stol(w) & 0xFFFF;
		return blez;
	}
	static OP* makeOP(word w) {
		BLEZ* blez = new BLEZ();
		word r = (w >> 21) & 0x1F;
		blez->rs = parseRegister(r);
		blez->imm = w & 0xFFFF;
		return blez;
	}
};

class BGTZ : public OP {
	string sop = "bgtz";
	word wop = 0x1C000000;
	Reg *rs;
	word imm;

	BGTZ() {}
public:
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + immtos(imm, 16);
	}
	virtual word toCode() {
		return wop | (rs->ID & 0x1F) | imm;
	}
	virtual word runOP() {
		PC += 4;
		if (rs->val > 0) PC += imm * 4;
		return 0;
	}
	static OP* makeOP(string s) {
		BGTZ* bgtz = new BGTZ();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			bgtz->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		string w = s.substr(j + 1);
		bgtz->imm = stol(w) & 0xFFFF;
		return bgtz;
	}
	static OP* makeOP(word w) {
		BGTZ* bgtz = new BGTZ();
		word r = (w >> 21) & 0x1F;
		bgtz->rs = parseRegister(r);
		bgtz->imm = w & 0xFFFF;
		return bgtz;
	}
};

class ADDI : public OPII {
	string sop = "addi";
	word wop = 0x20000000;

	ADDI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPII::toAssembly();
	}
	virtual word toCode() {
		return wop | OPII::toCode();
	}
	virtual word runOP() {
		rt->val = rs->val + imm;
		return 0;
	}
	static OP* makeOP(string s) {
		ADDI* addi = new ADDI();
		if (addi->parseAssembly(s))
			return addi;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		ADDI* addi = new ADDI();
		addi->parseCode(w);
		return addi;
	}
};

class ADDIU : public OPII {
	string sop = "addiu";
	word wop = 0x24000000;

	ADDIU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPII::toAssembly();
	}
	virtual word toCode() {
		return wop | OPII::toCode();
	}
	virtual word runOP() {
		rt->val = (unsigned)rs->val + (unsigned)imm;
		return 0;
	}
	static OP* makeOP(string s) {
		ADDIU* addiu = new ADDIU();
		if (addiu->parseAssembly(s))
			return addiu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		ADDIU* addiu = new ADDIU();
		addiu->parseCode(w);
		return addiu;
	}
};

class SLTI : public OPII {
	string sop = "slti";
	word wop = 0x28000000;

	SLTI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPII::toAssembly();
	}
	virtual word toCode() {
		return wop | OPII::toCode();
	}
	virtual word runOP() {
		rt->val = 0;
		if (rs->val < imm) rt->val = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		SLTI* slti = new SLTI();
		if (slti->parseAssembly(s))
			return slti;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SLTI* slti = new SLTI();
		slti->parseCode(w);
		return slti;
	}
};

class SLTIU : public OPII {
	string sop = "sltiu";
	word wop = 0x2C000000;

	SLTIU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPII::toAssembly();
	}
	virtual word toCode() {
		return wop | OPII::toCode();
	}
	virtual word runOP() {
		rt->val = 0;
		if ((unsigned)rs->val < (unsigned)imm) rt->val = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		SLTIU* sltiu = new SLTIU();
		if (sltiu->parseAssembly(s))
			return sltiu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SLTIU* sltiu = new SLTIU();
		sltiu->parseCode(w);
		return sltiu;
	}
};

class ANDI : public OPII {
	string sop = "andi";
	word wop = 0x30000000;

	ANDI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPII::toAssembly();
	}
	virtual word toCode() {
		return wop | OPII::toCode();
	}
	virtual word runOP() {
		rt->val = rs->val & imm;
		return 0;
	}
	static OP* makeOP(string s) {
		ANDI* andi = new ANDI();
		if (andi->parseAssembly(s))
			return andi;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		ANDI* andi = new ANDI();
		andi->parseCode(w);
		return andi;
	}
};

class ORI : public OPII {
	string sop = "ori";
	word wop = 0x34000000;

	ORI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPII::toAssembly();
	}
	virtual word toCode() {
		return wop | OPII::toCode();
	}
	virtual word runOP() {
		rt->val = rs->val + imm;
		return 0;
	}
	static OP* makeOP(string s) {
		ORI* ori = new ORI();
		if (ori->parseAssembly(s))
			return ori;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		ORI* ori = new ORI();
		ori->parseCode(w);
		return ori;
	}
};

class XORI : public OPII {
	string sop = "xori";
	word wop = 0x38000000;

	XORI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPII::toAssembly();
	}
	virtual word toCode() {
		return wop | OPII::toCode();
	}
	virtual word runOP() {
		rt->val = rs->val ^ imm;
		return 0;
	}
	static OP* makeOP(string s) {
		XORI* xori = new XORI();
		if (xori->parseAssembly(s))
			return xori;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		XORI* xori = new XORI();
		xori->parseCode(w);
		return xori;
	}
};

class LUI : public OP {
	string sop = "lui";
	word wop = 0x3C000000;
	Reg *rt;
	word imm;

	LUI() {}
public:
	virtual string toAssembly() {
		return sop + " " + rt->name + "," + immtos(imm, 16);
	}
	virtual word toCode() {
		return wop | (rt->ID << 16) | imm;
	}
	virtual word runOP() {
		imm = imm << 16;
		rt->val = imm;
		return 0;
	}
	static OP* makeOP(string s) {
		LUI* lui = new LUI();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			lui->rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		string w = s.substr(j + 1);
		lui->imm = stol(w) & 0xFFFF;
		return lui;
	}
	static OP* makeOP(word w) {
		LUI* lui = new LUI();
		word r = (w >> 16) & 0x1F;
		lui->rt = parseRegister(r);
		lui->imm = w & 0xFFFF;
		return lui;
	}
};

class LB : public OPILS {
	string sop = "lb";
	word wop = 0x80000000;

	LB() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		rt->val = MEM[rs->val + imm];
		return 0;
	}
	static OP* makeOP(string s) {
		LB* lb = new LB();
		if (lb->parseAssembly(s))
			return lb;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		LB* lb = new LB();
		lb->parseCode(w);
		return lb;
	}
};

class LH : public OPILS {
	string sop = "lh";
	word wop = 0x84000000;

	LH() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		rt->val = (MEM[rs->val + imm] << 8) | (MEM[rs->val + imm + 1]);
		return 0;
	}
	static OP* makeOP(string s) {
		LH* lh = new LH();
		if (lh->parseAssembly(s))
			return lh;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		LH* lh = new LH();
		lh->parseCode(w);
		return lh;
	}
};

class LWL : public OPILS {
	string sop = "lwl";
	word wop = 0x88000000;

	LWL() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		rt->val = (MEM[rs->val + imm] << 8) | (MEM[rs->val + imm + 1] );
		return 0;
	}
	static OP* makeOP(string s) {
		LWL* lwl = new LWL();
		if (lwl->parseAssembly(s))
			return lwl;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		LWL* lwl = new LWL();
		lwl->parseCode(w);
		return lwl;
	}
};

class LW : public OPILS {
	string sop = "lw";
	word wop = 0x8C000000;

	LW() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		rt->val = (MEM[rs->val + imm] << 24) | (MEM[rs->val + imm + 1] << 16) | (MEM[rs->val + imm + 2] << 8) | (MEM[rs->val + imm + 3]);
		return 0;
	}
	static OP* makeOP(string s) {
		LW* lw = new LW();
		if (lw->parseAssembly(s))
			return lw;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		LW* lw = new LW();
		lw->parseCode(w);
		return lw;
	}
};

class LBU : public OPILS {
	string sop = "lbu";
	word wop = 0x90000000;

	LBU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		rt->val = (unsigned)(MEM[rs->val + imm]);
		return 0;
	}
	static OP* makeOP(string s) {
		LBU* lbu = new LBU();
		if (lbu->parseAssembly(s))
			return lbu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		LBU* lbu = new LBU();
		lbu->parseCode(w);
		return lbu;
	}
};

class LHU : public OPILS {
	string sop = "lhu";
	word wop = 0x94000000;

	LHU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		rt->val = (unsigned)((MEM[rs->val + imm] << 8) | (MEM[rs->val + imm + 1]));
		return 0;
	}
	static OP* makeOP(string s) {
		LHU* lhu = new LHU();
		if (lhu->parseAssembly(s))
			return lhu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		LHU* lhu = new LHU();
		lhu->parseCode(w);
		return lhu;
	}
};

class LWR : public OPILS {
	string sop = "lwr";
	word wop = 0x98000000;

	LWR() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		rt->val = (MEM[rs->val + imm + 2] << 8) | (MEM[rs->val + imm + 3]);
		return 0;
	}
	static OP* makeOP(string s) {
		LWR* lwr = new LWR();
		if (lwr->parseAssembly(s))
			return lwr;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		LWR* lwr = new LWR();
		lwr->parseCode(w);
		return lwr;
	}
};

class SB : public OPILS {
	string sop = "sb";
	word wop = 0xA0000000;

	SB() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		MEM[rs->val + imm] = rt->val & 0x000F;
		return 0;
	}
	static OP* makeOP(string s) {
		SB* sb = new SB();
		if (sb->parseAssembly(s))
			return sb;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SB* sb = new SB();
		sb->parseCode(w);
		return sb;
	}
};

class SH : public OPILS {
	string sop = "sh";
	word wop = 0xA4000000;

	SH() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		MEM[rs->val + imm	 ] = (rt->val >> 8) & 0x000F;
		MEM[rs->val + imm + 1] = (rt->val	  ) & 0x000F;
		return 0;
	}
	static OP* makeOP(string s) {
		SH* sh = new SH();
		if (sh->parseAssembly(s))
			return sh;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SH* sh = new SH();
		sh->parseCode(w);
		return sh;
	}
};

class SWL : public OPILS {
	string sop = "swl";
	word wop = 0xA8000000;

	SWL() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		MEM[rs->val + imm	 ] = (rt->val >> 24) & 0x000F;
		MEM[rs->val + imm + 1] = (rt->val >> 16) & 0x000F;
		return 0;
	}
	static OP* makeOP(string s) {
		SWL* swl = new SWL();
		if (swl->parseAssembly(s))
			return swl;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SWL* swl = new SWL();
		swl->parseCode(w);
		return swl;
	}
};

class SW : public OPILS {
	string sop = "sw";
	word wop = 0xAC000000;

	SW() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		MEM[rs->val + imm	 ] = (rt->val >> 24) & 0x000F;
		MEM[rs->val + imm + 1] = (rt->val >> 16) & 0x000F;
		MEM[rs->val + imm + 2] = (rt->val >>  8) & 0x000F;
		MEM[rs->val + imm + 3] = (rt->val	   ) & 0x000F;
		return 0;
	}
	static OP* makeOP(string s) {
		SW* sw = new SW();
		if (sw->parseAssembly(s))
			return sw;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SW* sw = new SW();
		sw->parseCode(w);
		return sw;
	}
};

class SWR : public OPILS {
	string sop = "swr";
	word wop = 0xB8000000;

	SWR() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		MEM[rs->val + imm	 ] = (rt->val >> 8) & 0x000F;
		MEM[rs->val + imm + 1] = (rt->val) & 0x000F;
		return 0;
	}
	static OP* makeOP(string s) {
		SWR* swr = new SWR();
		if (swr->parseAssembly(s))
			return swr;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SWR* swr = new SWR();
		swr->parseCode(w);
		return swr;
	}
};

class LL : public OPILS {
	string sop = "ll";
	word wop = 0xC0000000;

	LL() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		rt->val = (MEM[rs->val + imm] << 24) | (MEM[rs->val + imm + 1] << 16) | (MEM[rs->val + imm + 2] << 8) | (MEM[rs->val + imm + 3]);
		return 0;
	}
	static OP* makeOP(string s) {
		LL* ll = new LL();
		if (ll->parseAssembly(s))
			return ll;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		LL* ll = new LL();
		ll->parseCode(w);
		return ll;
	}
};

class SC : public OPILS {
	string sop = "sc";
	word wop = 0xE0000000;

	SC() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPILS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPILS::toCode();
	}
	virtual word runOP() {
		MEM[rs->val + imm] = (rt->val >> 24) & 0x000F;
		MEM[rs->val + imm + 1] = (rt->val >> 16) & 0x000F;
		MEM[rs->val + imm + 2] = (rt->val >> 8) & 0x000F;
		MEM[rs->val + imm + 3] = (rt->val) & 0x000F;
		rt->val = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		SC* sc = new SC();
		if (sc->parseAssembly(s))
			return sc;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SC* sc = new SC();
		sc->parseCode(w);
		return sc;
	}
};
