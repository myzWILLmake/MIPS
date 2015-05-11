
class OPI1 : public OP {
protected:
	Reg *rs;
	word imm;

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
		string w = s.substr(j + 1);
		imm = stol(w);
		return true;
	}
	void parseCode(word w) {
		word r = (w >> 21) & 0x1F;
		rs = parseRegister(r);
		imm = w;
	}
	virtual string toAssembly() {
		return rs->name + "," + immtos(imm, 16);
	}
	virtual word toCode() {
		return (rs->ID << 21) | imm;
	}

};

class BLTZ : public OPI1 {
	string sop = "bltz";
	word wop = 0x04000000;

	BLTZ() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		PC += 4;
		if (rs->val < 0) PC += imm * 4; 
		return 0;
	}
	static OP* makeOP(string s) {
		BLTZ* bltz = new BLTZ();
		if (bltz->parseAssembly(s))
			return bltz;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		BLTZ* bltz = new BLTZ();
		bltz->parseCode(w);
		return bltz;
	}
};

class BGEZ : public OPI1 {
	string sop = "bgez";
	word wop = 0x04010000;

	BGEZ() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		PC += 4;
		if (rs->val >= 0) PC += imm * 4;
		return 0;
	}
	static OP* makeOP(string s) {
		BGEZ* bgez = new BGEZ();
		if (bgez->parseAssembly(s))
			return bgez;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		BGEZ* bgez = new BGEZ();
		bgez->parseCode(w);
		return bgez;
	}
};

class TGEI : public OPI1 {
	string sop = "tgei";
	word wop = 0x04080000;
	
	TGEI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		if (rs->val >= imm) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TGEI* tgei = new TGEI();
		if (tgei->parseAssembly(s))
			return tgei;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TGEI* tgei = new TGEI();
		tgei->parseCode(w);
		return tgei;
	}
};

class TGEIU : public OPI1 {
	string sop = "tgeiu";
	word wop = 0x04090000;

	TGEIU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		if ((unsigned)rs->val >= (unsigned)imm) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TGEIU* tgeiu = new TGEIU();
		if (tgeiu->parseAssembly(s))
			return tgeiu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TGEIU* tgeiu = new TGEIU();
		tgeiu->parseCode(w);
		return tgeiu;
	}
};

class TLTI : public OPI1 {
	string sop = "tlti";
	word wop = 0x040A0000;

	TLTI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		if (rs->val < imm) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TLTI* tlti = new TLTI();
		if (tlti->parseAssembly(s))
			return tlti;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TLTI* tlti = new TLTI();
		tlti->parseCode(w);
		return tlti;
	}
};

class TLTIU : public OPI1 {
	string sop = "tltiu";
	word wop = 0x040B0000;

	TLTIU() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		if ((unsigned)rs->val < (unsigned)imm) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TLTIU* tltiu = new TLTIU();
		if (tltiu->parseAssembly(s))
			return tltiu;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TLTIU* tltiu = new TLTIU();
		tltiu->parseCode(w);
		return tltiu;
	}
};

class TEQI : public OPI1 {
	string sop = "teqi";
	word wop = 0x040C0000;

	TEQI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		if (rs->val == imm) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TEQI* teqi = new TEQI();
		if (teqi->parseAssembly(s))
			return teqi;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TEQI* teqi = new TEQI();
		teqi->parseCode(w);
		return teqi;
	}
};

class TNEI : public OPI1 {
	string sop = "tnei";
	word wop = 0x040E0000;

	TNEI() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		if (rs->val != imm) EXP = 1;
		return 0;
	}
	static OP* makeOP(string s) {
		TNEI* tnei = new TNEI();
		if (tnei->parseAssembly(s))
			return tnei;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		TNEI* tnei = new TNEI();
		tnei->parseCode(w);
		return tnei;
	}
};

class BLTZAL : public OPI1 {
	string sop = "bltzal";
	word wop = 0x04100000;

	BLTZAL() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		PC += 4;
		if (rs->val < 0) {
			PC += imm * 4; 
			$ra->val = PC;
		}
		return 0;
	}
	static OP* makeOP(string s) {
		BLTZAL* bltzal = new BLTZAL();
		if (bltzal->parseAssembly(s))
			return bltzal;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		BLTZAL* bltzal = new BLTZAL();
		bltzal->parseCode(w);
		return bltzal;
	}
};

class BGEZAL : public OPI1 {
	string sop = "bgezal";
	word wop = 0x04110000;

	BGEZAL() {}
public:
	virtual string toAssembly() {
		return sop + " " + OPI1::toAssembly();
	}
	virtual word toCode() {
		return wop | OPI1::toCode();
	}
	virtual word runOP() {
		PC += 4;
		if (rs->val >= 0) {
			PC += imm * 4;
			$ra->val = PC;
		}
		return 0;
	}
	static OP* makeOP(string s) {
		BGEZAL* bgezal = new BGEZAL();
		if (bgezal->parseAssembly(s))
			return bgezal;
		else
			return NULL;
	}
	static OP* makeOP(word w) {
		BGEZAL* bgezal = new BGEZAL();
		bgezal->parseCode(w);
		return bgezal;
	}
};
