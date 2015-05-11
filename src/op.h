
class OP {
public:
	virtual word toCode() = 0;
	virtual string toAssembly() = 0;
	virtual word runOP() = 0;
};

string immtos(long w, int len) {
	char buf[10];
	if (w & (1 << (len - 1)))
		w |= 0xFFFFFFFF<< (32 - len);
	return ltoa(w, buf, 10);
}

string wtos(word w) {
	string s;
	for (int i = 0; i < 32; ++i)
	if (w & (1 << (31 - i)))
		s += "1";
	else
		s += "0";
	return s;
}

word stow(string s) {
	word w = 0;
	for (int i = 0; i < 32; ++i)
	if (s[i] == '1')
		w |= (1 << (31 - i));
	return w;
}
