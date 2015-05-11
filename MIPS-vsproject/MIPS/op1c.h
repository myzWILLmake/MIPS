
class MADD : public OP {
	string sop = "madd";
	word wop = 0x70000000;
	Reg *rs, *rt;

	MADD() {}
public:
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + rt->name;
	}
	virtual word toCode() {
		return wop | (rs->ID << 21) | (rt->ID << 16);
	}
	virtual word runOP() {
		HI = (((rs->val * rt->val) + ((HI << 32) | LO)) >> 32)| 0xFFFF;
		LO = ((rs->val * rt->val) + ((HI << 32) | LO)) | 0xFFFF;
		return 0;
	}
	static OP* makeOP(string s) {
		MADD* madd = new MADD();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			madd->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(j + 1);
		it = regs.find(r);
		if (it != regs.end())
			madd->rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return madd;
	}
	static OP* makeOP(word w) {
		MADD* madd = new MADD();
		word r = (w >> 21) & 0x1F;
		madd->rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		madd->rt = parseRegister(r);
		return madd;
	}
};

class MADDU : public OP {
	string sop = "maddu";
	word wop = 0x70000001;
	Reg *rs, *rt;

	MADDU() {}
public:
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + rt->name;
	}
	virtual word toCode() {
		return wop | (rs->ID << 21) | (rt->ID << 16);
	}
	virtual word runOP() {
		HI = ((((unsigned)rs->val * (unsigned)rt->val) + (((unsigned)HI << 32) | (unsigned)LO)) >> 32) | 0xFFFF;
		LO = (((unsigned)rs->val * (unsigned)rt->val) + (((unsigned)HI << 32) | (unsigned)LO)) | 0xFFFF;
		return 0;
	}
	static OP* makeOP(string s) {
		MADDU* maddu = new MADDU();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			maddu->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(j + 1);
		it = regs.find(r);
		if (it != regs.end())
			maddu->rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return maddu;
	}
	static OP* makeOP(word w) {
		MADDU* maddu = new MADDU();
		word r = (w >> 21) & 0x1F;
		maddu->rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		maddu->rt = parseRegister(r);
		return maddu;
	}
};

class MUL : public OP {
	string sop = "mul";
	word wop = 0x70000002;
	Reg *rs, *rt, *rd;

	MUL() {}
public:
	virtual string toAssembly() {
		return sop + " " + rd->name + "," + rs->name + "," + rt->name;
	}
	virtual word toCode() {
		return wop | (rd->ID << 21) | (rs->ID << 16) | (rt->ID << 11);
	}
	virtual word runOP() {
		rd->val = rs->val * rt->val;
		return 0;
	}
	static OP* makeOP(string s) {
		MUL* mul = new MUL();
		int i = s.find(' ');
		int j = s.find(',');
		int k = s.rfind(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			mul->rd = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(j + 1, k - j - 1);
		it = regs.find(r);
		if (it != regs.end())
			mul->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(k + 1);
		it = regs.find(r);
		if (it != regs.end())
			mul->rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return mul;
	}
	static OP* makeOP(word w) {
		MUL* mul = new MUL();
		word r = (w >> 21) & 0x1F;
		mul->rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		mul->rt = parseRegister(r);
		r = (w >> 11) & 0x1F;
		mul->rd = parseRegister(r);
		return mul;
	}
};

class MSUB : public OP {
	string sop = "msub";
	word wop = 0x70000004;
	Reg *rs, *rt;

	MSUB() {}
public:
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + rt->name;
	}
	virtual word toCode() {
		return wop | (rs->ID << 21) | (rt->ID << 16);
	}
	virtual word runOP() {
		HI = (((rs->val * rt->val) - ((HI << 32) | LO)) >> 32) | 0xFFFF;
		LO = ((rs->val * rt->val) - ((HI << 32) | LO)) | 0xFFFF;
		return 0;
	}
	static OP* makeOP(string s) {
		MSUB* msub = new MSUB();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			msub->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(j + 1);
		it = regs.find(r);
		if (it != regs.end())
			msub->rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return msub;
	}
	static OP* makeOP(word w) {
		MSUB* msub = new MSUB();
		word r = (w >> 21) & 0x1F;
		msub->rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		msub->rt = parseRegister(r);
		return msub;
	}
};

class MSUBU : public OP {
	string sop = "msubu";
	word wop = 0x70000005;
	Reg *rs, *rt;

	MSUBU() {}
public:
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + rt->name;
	}
	virtual word toCode() {
		return wop | (rs->ID << 21) | (rt->ID << 16);
	}
	virtual word runOP() {
		HI = ((((unsigned)rs->val * (unsigned)rt->val) + (((unsigned)HI << 32) | (unsigned)LO)) >> 32) | 0xFFFF;
		LO = (((unsigned)rs->val * (unsigned)rt->val) + (((unsigned)HI << 32) | (unsigned)LO)) | 0xFFFF;
		return 0;
	}
	static OP* makeOP(string s) {
		MSUBU* msubu = new MSUBU();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			msubu->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(j + 1);
		it = regs.find(r);
		if (it != regs.end())
			msubu->rt = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return msubu;
	}
	static OP* makeOP(word w) {
		MSUBU* msubu = new MSUBU();
		word r = (w >> 21) & 0x1F;
		msubu->rs = parseRegister(r);
		r = (w >> 16) & 0x1F;
		msubu->rt = parseRegister(r);
		return msubu;
	}
};

class CLZ : public OP {
	string sop = "clz";
	word wop = 0x70000020;
	Reg *rs, *rd;

	CLZ() {}
public:
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + rd->name;
	}
	virtual word toCode() {
		return wop | (rs->ID << 21) | (rd->ID << 11);
	}
	virtual word runOP() {
		int tmpval = rs->val;
		int count = 0;
		while (tmpval >= 0){
			tmpval = tmpval << 1;
			count++;
			if (count == 32) break;
		}
		rd->val = count;
		return 0;
	}
	static OP* makeOP(string s) {
		CLZ* clz = new CLZ();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			clz->rd = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(j + 1);
		it = regs.find(r);
		if (it != regs.end())
			clz->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return clz;
	}
	static OP* makeOP(word w) {
		CLZ* clz = new CLZ();
		word r = (w >> 21) & 0x1F;
		clz->rd = parseRegister(r);
		r = (w >> 16) & 0x1F;
		clz->rs = parseRegister(r);
		return clz;
	}
};

class CLO : public OP {
	string sop = "clo";
	word wop = 0x70000021;
	Reg *rs, *rd;

	CLO() {}
public:
	virtual string toAssembly() {
		return sop + " " + rs->name + "," + rd->name;
	}
	virtual word toCode() {
		return wop | (rs->ID << 21) | (rd->ID << 11);
	}
	virtual word runOP() {
		int tmpval = rs->val;
		int count = 0;
		while (tmpval < 0){
			tmpval = tmpval << 1;
			count++;
			if (count == 32) break;
		}
		rd->val = count;
		return 0;
	}
	static OP* makeOP(string s) {
		CLO* clo = new CLO();
		int i = s.find(' ');
		int j = s.find(',');
		string r = s.substr(i + 1, j - i - 1);
		map<string, Reg*>::iterator it = regs.find(r);
		if (it != regs.end())
			clo->rd = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		r = s.substr(j + 1);
		it = regs.find(r);
		if (it != regs.end())
			clo->rs = (*it).second;
		else {
			cout << "Wrong Register !" << endl;
			return NULL;
		}
		return clo;
	}
	static OP* makeOP(word w) {
		CLO* clo = new CLO();
		word r = (w >> 21) & 0x1F;
		clo->rd = parseRegister(r);
		r = (w >> 16) & 0x1F;
		clo->rs = parseRegister(r);
		return clo;
	}
};
