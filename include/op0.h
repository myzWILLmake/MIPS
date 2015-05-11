// standard Rtype
class OPR : public OP {
protected:
	Reg *rs, *rt, *rd;

	bool parseAssembly(string s) {
		int i = s.find(' ');
		int j = s.find(',');
		int k = s.rfind(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			rd = (*it).second;
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
		r = s.substr(k + 1);
		it = regs.find(r);
		if (it != regs.end())
			rt = (*it).second;
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
		r = (w >> 11) & 0x1F;
		rd = parseRegister(r);
	}
	virtual  word toCode() {
		return (rs->ID << 21) | (rt->ID << 16) | (rd->ID << 11);
	}
	virtual string toAssembly() {
		return rd->name + "," + rs->name + "," + rt->name;
	}

};
// Rtype shift
class OPRS : public OP {
protected:
	Reg *rt, *rd;
	word shamt;

	bool parseAssembly(string s) {
		int i = s.find(' ');
		int j = s.find(',');
		int k = s.rfind(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			rd = (*it).second;
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
		shamt = stol(w) & 0x1F;
		return true;
	}
	void parseCode(word w) {
		word r = (w >> 16) & 0x1F;
		rt = parseRegister(r);
		r = (w >> 11) & 0x1F;
		rd = parseRegister(r);
		shamt = (w >> 6) & 0x1F;
	}
	virtual word toCode() {
		return (rt->ID << 16) | (rd->ID << 11) | (shamt << 6);
	}
	virtual string toAssembly() {
		return rd->name + "," + rt->name + "," + immtos(shamt, 5);
	}
};
// Rtype mul/div/trap
class OPRMDT : public OP {
protected:
	Reg *rs, *rt;

	bool parseAssembly(string s) {
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return false;
		}
		r = s.substr(j + 1);
		it = regs.find(r);
		if (it != regs.end())
			rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return false;
		}
		return true;
	}
	void parseCode(word w) {
		int r = (w >> 21) & 0x1F;
		rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		rt = parseRegister(r);
	}
	virtual word toCode() {
		return (rs->ID << 21) | (rt->ID << 16);
	}
	virtual string toAssembly() {
		return rs->name + "," + rt->name;
	}
};

class SLL : public OPRS {
	string sop = "sll";
	word wop = 0x00000000;

	SLL() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRS::toCode();
	}
	virtual word runOP() {
		rd->val = rt->val << shamt;
		return 0;
	}
	static OP* makeOP(word w) {
		SLL* sll = new SLL();
		sll->parseCode(w);
		return sll;
	}
	static OP* makeOP(string s) {
		SLL* sll = new SLL();
		if (sll->parseAssembly(s))
			return sll;
		else
			return NULL;
	}
};

class SRL : public OPRS {
	string sop = "srl";
	word wop = 0x00000002;

	SRL() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRS::toCode();
	}
	virtual word runOP() {
		rd->val = (unsigned)rt->val >> shamt;
		return 0;
	}
	static OP* makeOP(word w) {
		SRL* srl = new SRL();
		srl->parseCode(w);
		return srl;
	}
	static OP* makeOP(string s) {
		SRL* srl = new SRL();
		if (srl->parseAssembly(s))
			return srl;
		else
			return NULL;
	}
};

class SRA : public OPRS {
	string sop = "sra";
	word wop = 0x00000003;

	SRA() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRS::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRS::toCode();
	}
	virtual word runOP() {
		rd->val = rt->val >> shamt;
		return 0;
	}
	static OP* makeOP(word w) {
		SRA* sra = new SRA();
		sra->parseCode(w);
		return sra;
	}
	static OP* makeOP(string s) {
		SRA* sra = new SRA();
		if (sra->parseAssembly(s))
			return sra;
		else
			return NULL;
	}
};

class SLLV : public OPR {
	string sop = "sllv";
	word wop = 0x00000004;

	SLLV() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}	
	virtual word runOP() {
		rd->val = rt->val << rs->val;
		return 0;
	}
	static OP* makeOP(word w) {
		SLLV* sllv = new SLLV();
		sllv->parseCode(w);
		return sllv;
	}
	static OP* makeOP(string s) {
		SLLV* sllv = new SLLV();
		if (sllv->parseAssembly(s))
			return sllv;
		else
			return NULL;
	}
};

class SRLV : public OPR {
	string sop = "srlv";
	word wop = 0x00000006;
	
	SRLV() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = (unsigned)rt->val >> rs->val;
		return 0;
	}
	static OP* makeOP(word w) {
		SRLV* srlv = new SRLV();
		srlv->parseCode(w);
		return srlv;
	}
	static OP* makeOP(string s) {
		SRLV* srlv = new SRLV();
		if (srlv->parseAssembly(s))
			return srlv;
		else
			return NULL;
	}
};

class SRAV : public OPR {
	string sop = "srav";
	word wop = 0x00000007;

	SRAV() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = rt->val >> rs->val;
		return 0;
	}
	static OP* makeOP(word w) {
		SRAV* srav = new SRAV();
		srav->parseCode(w);
		return srav;
	}
	static OP* makeOP(string s) {
		SRAV* srav = new SRAV();
		if (srav->parseAssembly(s))
			return srav;
		else
			return NULL;
	}
};

class JR : public OP {
	string sop = "jr";
	word wop = 0x00000008;
	Reg *rs;

	JR() {}
public:
	virtual word toCode() {
		return wop | (rs->ID << 21);
	}
	virtual string toAssembly() {
		return sop + " " + rs->name;
	}
	virtual word runOP() {
		PC = rs->val;
		return 0;
	}
	static OP* makeOP(string s) {
		JR* jr = new JR();
		int i = s.find(' ');
		string r = s.substr(i + 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			jr->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return jr;
	}
	static OP* makeOP(word w) {
		JR* jr = new JR();
		word r = (w >> 21) & 0x1F;
		jr->rs = parseRegister(r);
		return jr;
	}
};

class JALR : public OP {
	string sop = "jalr";
	word wop = 0x00000009;
	Reg *rs, *rd;
	
	JALR() {}
public:
	virtual word toCode() {
		return wop | (rs->ID << 21) | (rd->ID << 16);
	}
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + rd->name;
	}
	virtual word runOP() {
		rd->val = PC + 4;
		PC = rs->val;
		return 0;
	}
	static OP* makeOP(string s) {
		JALR* jalr = new JALR();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			jalr->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(j + 1);
		it = regs.find(r);
		if (it != regs.end())
			jalr->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return jalr;
	}
	static OP* makeOP(word w) {
		JALR* jalr = new JALR();
		word r = (w >> 21) & 0x1F;
		jalr->rs = parseRegister(r);
		r = (w >> 11) & 0x1F;
		jalr->rd = parseRegister(r);
		return jalr;
	}
};

class MOVZ : public OPR {
	string sop = "movz";
	word wop = 0x0000000A;

	MOVZ() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		if (rt->val == 0) rd->val = rs->val;
		return 0;
	}
	static OP* makeOP(word w) {
		MOVZ* movz = new MOVZ();
		movz->parseCode(w);
		return movz;
	}
	static OP* makeOP(string s) {
		MOVZ* movz = new MOVZ();
		if (movz->parseAssembly(s))
			return movz;
		else
			return NULL;
	}
};

class MOVN : public OPR {
	string sop = "movn";
	word wop = 0x0000000B;
	
	MOVN() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		if (rt->val != 0) rd->val = rs->val;
		return 0;
	}
	static OP* makeOP(word w) {
		MOVN* movn = new MOVN();
		movn->parseCode(w);
		return movn;
	}
	static OP* makeOP(string s) {
		MOVN* movn = new MOVN();
		if (movn->parseAssembly(s))
			return movn;
		else
			return NULL;
	}
};

class MFHI : public OP {
	string sop = "mfhi";
	word wop = 0x00000010;
	Reg *rd;

	MFHI() {}
public:
	virtual word toCode() {
		return wop | (rd->ID << 11);
	}
	virtual string toAssembly() {
		return sop + " " + rd->name;
	}
	virtual word runOP() {
		rd->val = HI;
		return 0;
	}
	static OP* makeOP(string s) {
		MFHI* mfhi = new MFHI();
		int i = s.find(' ');
		string r = s.substr(i + 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			mfhi->rd = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return mfhi;
	}
	static OP* makeOP(word w) {
		MFHI* mfhi = new MFHI();
		word r = (w >> 11) & 0x1F;
		mfhi->rd = parseRegister(r);
		return mfhi;
	}
};

class MTHI : public OP {
	string sop = "mthi";
	word wop = 0x00000011;
	Reg *rs;

	MTHI() {}
public:
	virtual word toCode() {
		return wop | (rs->ID << 21);
	}
	virtual string toAssembly() {
		return sop + " " + rs->name;
	}
	virtual word runOP() {
		HI = rs->val;
		return 0;
	}
	static OP* makeOP(string s) {
		MTHI* mthi = new MTHI();
		int i = s.find(' ');
		string r = s.substr(i + 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			mthi->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return mthi;
	}
	static OP* makeOP(word w) {
		MTHI* mthi = new MTHI();
		word r = (w >> 21) & 0x1F;
		mthi->rs = parseRegister(r);
		return mthi;
	}
};

class MFLO : public OP {
	string sop = "mflo";
	word wop = 0x00000012;
	Reg *rd;

	MFLO() {}
public:
	virtual word toCode() {
		return wop | (rd->ID << 11);
	}
	virtual string toAssembly() {
		return sop + " " + rd->name;
	}
	virtual word runOP() {
		rd->val = LO;
		return 0;
	}
	static OP* makeOP(string s) {
		MFLO* mflo = new MFLO();
		int i = s.find(' ');
		string r = s.substr(i + 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			mflo->rd = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return mflo;
	}
	static OP* makeOP(word w) {
		MFLO* mflo = new MFLO();
		word r = (w >> 11) & 0x1F;
		mflo->rd = parseRegister(r);
		return mflo;
	}
};

class MTLO : public OP {
	string sop = "mtlo";
	word wop = 0x00000013;
	Reg *rs;
	
	MTLO() {}
public:
	virtual word toCode() {
		return wop | (rs->ID << 21);
	}
	virtual string toAssembly() {
		return sop + " " + rs->name;
	}
	virtual word runOP() {
		LO = rs->val;
		return 0;
	}
	static OP* makeOP(string s) {
		MTLO* mtlo = new MTLO();
		int i = s.find(' ');
		string r = s.substr(i + 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			mtlo->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return mtlo;
	}
	static OP* makeOP(word w) {
		MTLO* mtlo = new MTLO();
		word r = (w >> 21) & 0x1F;
		mtlo->rs = parseRegister(r);
		return mtlo;
	}
};

class MULT : public OPRMDT {
	string sop = "mult";
	word wop = 0x00000018;

	MULT() {}
public:
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word runOP() {
		HI = ((rs->val * rt->val) >> 32) & 0xFFFF;
		LO = ((rs->val * rt->val)      ) & 0XFFFF;
		return 0;
	}
	static OP* makeOP(string s) {
		MULT* mult = new MULT();
		if (mult->parseAssembly(s))
			return mult;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		MULT* mult = new MULT();
		mult->parseCode(w);
		return mult;
	}
};

class MULTU : public OPRMDT {
	string sop = "multu";
	word wop = 0x00000019;
	
	MULTU() {}
public:
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word runOP() {
		HI = (((unsigned)rs->val * (unsigned)rt->val) >> 32) & 0xFFFF;
		LO = (((unsigned)rs->val * (unsigned)rt->val)      ) & 0XFFFF;
		return 0;
	}
	static OP* makeOP(string s) {
		MULTU* multu = new MULTU();
		if (multu->parseAssembly(s))
			return multu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		MULTU* multu = new MULTU();
		multu->parseCode(w);
		return multu;
	}
};

class DIV : public OPRMDT {
	string sop = "div";
	word wop = 0x0000001A;
	
	DIV() {}
public:
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word runOP() {
		LO = rs->val / rt->val;
		HI = rs->val % rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		DIV* div = new DIV();
		if (div->parseAssembly(s))
			return div;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		DIV* div = new DIV();
		div->parseCode(w);
		return div;
	}
};

class DIVU : public OPRMDT {
	string sop = "divu";
	word wop = 0x0000001B;
	
	DIVU() {}
public:
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word runOP() {
		LO = (unsigned)rs->val / (unsigned)rt->val;
		HI = (unsigned)rs->val % (unsigned)rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		DIVU* divu = new DIVU();
		if (divu->parseAssembly(s))
			return divu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		DIVU* divu = new DIVU();
		divu->parseCode(w);
		return divu;
	}
};

class ADD : public OPR {
	string sop = "add";
	word wop = 0x00000020;
	
	ADD() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = rs->val + rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		ADD* add = new ADD();
		if (add->parseAssembly(s))
			return add;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		ADD* add = new ADD();
		add->parseCode(w);
		return add;
	}
};

class ADDU : public OPR {
	string sop = "addu";
	word wop = 0x00000021;

	ADDU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = (unsigned)rs->val + (unsigned)rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		ADDU* addu = new ADDU();
		if (addu->parseAssembly(s))
			return addu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		ADDU* addu = new ADDU();
		addu->parseCode(w);
		return addu;
	}
};

class SUB : public OPR {
	string sop = "sub";
	word wop = 0x00000022;

	SUB() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = rs->val - rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		SUB* sub = new SUB();
		if (sub->parseAssembly(s))
			return sub;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SUB* sub = new SUB();
		sub->parseCode(w);
		return sub;
	}
};

class SUBU : public OPR {
	string sop = "subu";
	word wop = 0x00000023;

	SUBU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = (unsigned)rs->val - (unsigned)rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		SUBU* subu = new SUBU();
		if (subu->parseAssembly(s))
			return subu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SUBU* subu = new SUBU();
		subu->parseCode(w);
		return subu;
	}
};

class AND : public OPR {
	string sop = "and";
	word wop = 0x00000024;

	AND() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = rs->val & rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		AND* and = new AND();
		if (and->parseAssembly(s))
			return and;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		AND* and = new AND();
		and->parseCode(w);
		return and;
	}
};

class OR : public OPR {
	string sop = "or";
	word wop = 0x00000025;
	
	OR() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = rs->val | rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		OR* or = new OR();
		if (or->parseAssembly(s))
			return or;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		OR* or = new OR();
		or->parseCode(w);
		return or;
	}
};

class XOR : public OPR {
	string sop = "xor";
	word wop = 0x00000026;
	
	XOR() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = rs->val ^ rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		XOR* xor = new XOR();
		if (xor->parseAssembly(s))
			return xor;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		XOR* xor = new XOR();
		xor->parseCode(w);
		return xor;
	}
};

class NOR : public OPR {
	string sop = "nor";
	word wop = 0x00000027;
	
	NOR() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = ~(rs->val | rt->val);
		return 0;
	}
	static OP* makeOP(string s) {
		NOR* nor = new NOR();
		if (nor->parseAssembly(s))
			return nor;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		NOR* nor = new NOR();
		nor->parseCode(w);
		return nor;
	}
};

class SLT : public OPR {
	string sop = "slt";
	word wop = 0x0000002A;
	
	SLT() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = 0;
		if (rs->val < rt->val) rd->val = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		SLT* slt = new SLT();
		if (slt->parseAssembly(s))
			return slt;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SLT* slt = new SLT();
		slt->parseCode(w);
		return slt;
	}
};

class SLTU : public OPR {
	string sop = "sltu";
	word wop = 0x0000002B;

	SLTU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPR::toAssembly();
	}
	virtual word toCode() {
		return wop | OPR::toCode();
	}
	virtual word runOP() {
		rd->val = 0;
		if ((unsigned)rs->val < (unsigned)rt->val) rd->val = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		SLTU* sltu = new SLTU();
		if (sltu->parseAssembly(s))
			return sltu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		SLTU* sltu = new SLTU();
		sltu->parseCode(w);
		return sltu;
	}
};

class TGE : public OPRMDT {
	string sop = "tge";
	word wop = 0x00000030;

	TGE() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual word runOP() {
		if (rs->val >= rt->val) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TGE* tge = new TGE();
		if (tge->parseAssembly(s))
			return tge;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TGE* tge = new TGE();
		tge->parseCode(w);
		return tge;
	}
};

class TGEU : public OPRMDT {
	string sop = "tgeu";
	word wop = 0x00000031;

	TGEU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual word runOP() {
		if ((unsigned)rs->val >= (unsigned)rt->val) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TGEU* tgeu = new TGEU();
		if (tgeu->parseAssembly(s))
			return tgeu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TGEU* tgeu = new TGEU();
		tgeu->parseCode(w);
		return tgeu;
	}
};

class TLT : public OPRMDT {
	string sop = "tlt";
	word wop = 0x00000032;
	
	TLT() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual word runOP() {
		if (rs->val < rt->val) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TLT* tlt = new TLT();
		if (tlt->parseAssembly(s))
			return tlt;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TLT* tlt = new TLT();
		tlt->parseCode(w);
		return tlt;
	}
};

class TLTU : public OPRMDT {
	string sop = "tltu";
	word wop = 0x00000033;
	
	TLTU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual word runOP() {
		if ((unsigned)rs->val != (unsigned)rt->val) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TLTU* tltu = new TLTU();
		if (tltu->parseAssembly(s))
			return tltu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TLTU* tltu = new TLTU();
		tltu->parseCode(w);
		return tltu;
	}
};

class TEQ : public OPRMDT {
	string sop = "teq";
	word wop = 0x00000034;

	TEQ() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual word runOP() {
		if (rs->val == rt->val) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TEQ* teq = new TEQ();
		if (teq->parseAssembly(s))
			return teq;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TEQ* teq = new TEQ();
		teq->parseCode(w);
		return teq;
	}
};

class TNE : public OPRMDT {
	string sop = "tne";
	word wop = 0x00000036;
	
	TNE() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPRMDT::toAssembly();
	}
	virtual word toCode() {
		return wop | OPRMDT::toCode();
	}
	virtual word runOP() {
		if (rs->val != rt->val) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TNE* tne = new TNE();
		if (tne->parseAssembly(s))
			return tne;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TNE* tne = new TNE();
		tne->parseCode(w);
		return tne;
	}
};
