/*
	NTU CSIE Compiler 2011 Lab 4
	B97501046 Qing-Cheng Li

	generator.h
	The header file of generator.cc, used to generate .bass code.
*/
#ifndef _GENERATOR_H_
#define _GENERATOR_H_
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <stack>
#include <vector>
#include "token.h"

using namespace std;

class Generator{
	private:
		int labelCounter;
		int varCounter;
		ofstream fout;
		char *programName;
		stack< map<string,pair<int,int> > > tableStack;
		map<string,pair<int,int> > currentTable;
	public:
		Generator(char*,char*);
		~Generator();

		/*Program layout*/
		void header();	//header
		void eop();		//end of program

		void newScope();
		void endScope();

		void addId(string,int);
		int getIndex(string);
		int getDim(string);

		int getNewLabel();
		void putLabel(int);
		void jump(int);

		void ifne(int);
		void ifeq(int);
		void if_icmpne(int);
		void if_icmpeq(int);
		void if_icmpge(int);
		void if_icmpgt(int);
		void if_icmple(int);
		void if_icmplt(int);

		void newArray(int,vector<string>);

		void ldc(string str);
		void ldcInt(string str);

		void iload(int);
		void istore(int);
		void aload(int);
		void aaload();
		void iaload();
		void iastore();

		void iadd();
		void isub();
		void imul();
		void idiv();
		void ineg();

		void printString();	//call print(string) @ StdIO
		void printInt();	//call print(int) @ StdIO
		void println();		//call println @ StdIO
		void scan();		//call readInt @ StdIO
};

#endif
