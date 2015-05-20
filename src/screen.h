class screen{
public:
	int waitInput() {
		cout << ">>";
		return 0;
	}

	int wrongInput() {
		cout << "Error!Check your input." << endl;
		return 0;
	}

	int simMode() {
		OP* insset[MAX_INS];
		memset(MEM, 0, MAX_MEM * sizeof(char));
		memset(insset, NULL, MAX_INS * sizeof(OP*));
		PC = 0;
		HI = 0;
		LO = 0;
		EXP = 0;
		word insAmount = 0;
		cout << "What do you want to do?" << endl;
		cout << "1.Input instructions" << endl;
		cout << "2.Run an instruction" << endl;
		cout << "3.Check PC and other info" << endl;
		cout << "4.Check registers" << endl;
		cout << "5.Check memory" << endl;
		cout << "6.Reset" << endl;
		cout << "q.Quit simulator mode" << endl;
		while (1) {
			string s;
			waitInput();
			getline(cin, s);
			switch (s[0])
			{
			case '1': {
						  cout << "Input your instructions. Input `quit` to quit." << endl;
						  while (1) {
							  OP* op;
							  waitInput();
							  string sins;
							  getline(cin, sins);
							  if (sins == "quit") {
								  break;
							  }
							  op = MIPS::parse(sins);
							  if (op != NULL) {
								  insset[insAmount] = op;
								  insAmount++;
							  }
						  }
						  cout << "Instructions amount : " << insAmount << endl;
						  break;
						  
			}
			case '2': {
						  word tmpPC = PC;
						  if (insset[PC / 4] != NULL)
						  {
							  cout << "Run the code : " << insset[PC / 4]->toAssembly() << endl;
							  insset[PC / 4]->runOP();
							  if (PC == tmpPC) PC += 4;
						  }
						  else
						  {
							  cout << "NULL Code" << endl;
						  }
						  break;
			}
			case '3': {
						  cout << "PC = " << PC << "	HI = " << HI << "	LO = " << LO << "	EXP = " << EXP << endl;
						  break;
			}
			case '4': {
						  cout << "$zero = " << $zero->val << " ";
						  cout << "$at = " << $at->val << " ";
						  cout << "$v0 = " << $v0->val << " ";
						  cout << "$v1 = " << $v1->val <<endl;
						  cout << "$a0 = " << $a0->val << " ";
						  cout << "$a1 = " << $a1->val << " ";
						  cout << "$a2 = " << $a2->val << " ";
						  cout << "$a3 = " << $a3->val <<endl;
						  cout << "$t0 = " << $t0->val << " ";
						  cout << "$t1 = " << $t1->val << " ";
						  cout << "$t2 = " << $t2->val << " ";
						  cout << "$t3 = " << $t3->val <<endl;
						  cout << "$t4 = " << $t4->val << " ";
						  cout << "$t5 = " << $t5->val << " ";
						  cout << "$t6 = " << $t6->val << " ";
						  cout << "$t7 = " << $t7->val <<endl;
						  cout << "$s0 = " << $s0->val << " ";
						  cout << "$s1 = " << $s1->val << " ";
						  cout << "$s2 = " << $s2->val << " ";
						  cout << "$s3 = " << $s3->val <<endl;
						  cout << "$s4 = " << $s4->val << " ";
						  cout << "$s5 = " << $s5->val << " ";
						  cout << "$s6 = " << $s6->val << " ";
						  cout << "$s7 = " << $s7->val <<endl;
						  cout << "$t8 = " << $t8->val << " ";
						  cout << "$t9 = " << $t9->val << " ";
						  cout << "$k0 = " << $k0->val << " ";
						  cout << "$k1 = " << $k1->val <<endl;
						  cout << "$gp = " << $gp->val << " ";
						  cout << "$sp = " << $sp->val << " ";
						  cout << "$fp = " << $fp->val << " ";
						  cout << "$ra = " << $ra->val <<endl;
						  break;
			}
			case '5': {	  //这里规定内存0处不可使用
						  cout << "Input the address (input `quit` to quit)" << endl;
						  while (1) {
							  string s;

							  waitInput();
							  getline(cin, s);
							  if (s == "quit") break;
							  word addr=atol(s.c_str());
							  if (addr > 0 && addr < MAX_MEM) {
								  cout << "MEM[" << addr << "] = " << (int)MEM[addr] << endl;
							  }
							  else
							  {
								  cout << "Error! Wrong Address." << endl;
							  }
							  
						  }
						  break;
			}
			case '6': {	
						  for (int i = 0; i < insAmount; i++) {
							  OP* tmp = insset[i];
							  delete tmp;
						  }
						  memset(MEM, 0, MAX_MEM * sizeof(char));
						  memset(insset, NULL, MAX_INS * sizeof(OP*));
						  PC = 0;
						  HI = 0;
						  LO = 0;
						  EXP = 0;
						  insAmount = 0;
						  break;
			}
			case 'q': {	
						  cout << "Choose the mode:" << endl;
						  cout << "1.Assembly" << endl;
						  cout << "2.Disassembly" << endl;
						  cout << "3.Simulator" << endl;
						  cout << "q.Quit" << endl;
						  return 0;
			}
			default:wrongInput();
			}
		}


	}

	int assMode() {
		cout << "Please input MIPS Assembly Language (Input `quit` to quit): " << endl;
		while (1) {
			OP* op;
			string s;

			waitInput();
			getline(cin, s);
			if (s == "quit")
			{
				cout << "Choose the mode:" << endl;
				cout << "1.Assembly" << endl;
				cout << "2.Disassembly" << endl;
				cout << "3.Simulator" << endl;
				cout << "q.Quit" << endl;
				return 0;
			}
			op = MIPS::parse(s);
			if (op != NULL)
			{
				cout << "Machine Code : " << hex << wtos(op->toCode()) << endl;
				delete op;
			}
		}
	}

	int disMode() {
		cout << "Please input Machine Code (Input `quit` to quit): " << endl;
		while (1) {
			OP* op;
			string s;

			waitInput();
			getline(cin, s);
			if (s == "quit")
			{	
				cout << "Choose the mode:" << endl;
				cout << "1.Assembly" << endl;
				cout << "2.Disassembly" << endl;
				cout << "3.Simulator" << endl;
				cout << "q.Quit" << endl;
				return 0;
			}
			op = MIPS::parse(s);
			if (op != NULL)
			{
				cout << "MIPS Assembly Language : " << op->toAssembly() << endl;
				delete op;
			}
		}
	}

	int welcome() {
		cout << "Choose the mode:" << endl;
		cout << "1.Assembly" << endl;
		cout << "2.Disassembly" << endl;
		cout << "3.Simulator" << endl;
		cout << "q.Quit" << endl;
		while (1){
			string s;
			waitInput();
			getline(cin, s);
			switch (s[0]) {
			case '1':assMode(); break;
			case '2':disMode(); break;
			case '3':simMode(); break;
			case 'q':exit(0); break;
			default:wrongInput();
			}
		}
	}
};
