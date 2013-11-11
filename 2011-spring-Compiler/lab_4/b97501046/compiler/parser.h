/*
   NTU CSIE Compiler 2011 Lab 4
   B97501046 Qing-Cheng Li

   parser.h
   The header file of parser ( LL(1) )
 */
#ifndef _PARSER_H_
#define _PARSER_H_
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "scanner.h"
#include "generator.h"

using namespace std;

class Parser{
	private:
		Scanner* scanner;
		Generator* coGen;
		bool pass;
		map<string,int> StringToTokenCode;
		Token* match(string,string);

		enum{
			TC_ADD=43,  //+
			TC_SUB=45,  //-
			TC_MUT=42,  //*
			TC_DIV=47,  ///
			TC_AND=256, //&&
			TC_OR=257,  //||
			TC_NOT=33,  //!
			TC_LT=60,   //<
			TC_GT=62,   //>
			TC_LE=258,  //<=
			TC_GE=259,  //>=
			TC_EQU=260, //==
			TC_NEQ=261, //!=
			TC_LP=40,   //(
			TC_RP=41,   //)
			TC_LS=91,   //[
			TC_RS=93,   //]
			TC_LC=123,  //{
			TC_RC=125,  //}
			TC_COM=44,  //,
			TC_SEM=59,  //;
			TC_EQ=61,   //=
			TC_INT=262, //int
			TC_IF=263,  //if
			TC_ELSE=264,//else
			TC_WHILE=265,//while
			TC_BRK=266, //break
			TC_CNT=267, //continue
			TC_SCAN=268,//scan
			TC_PRINT=269,//print
			TC_ID=270,  //id
			TC_NUM=271, //num
			TC_STR=272, //string
			TC_PRINTLN=273  //println, add for Lab4
		};

		/* Recursive Desent Parsing */
		bool Program();
		bool BlockStmt(int,int);
		bool VarDecl();
		vector<string> VarDeclNum();
		bool VarDeclId(vector<string>);
		string Var(int);
		bool VarArithExpr(int);
		bool Stmt(int,int);
		bool StmtElse(int,int);
		bool StmtVar();
		bool StmtPrintableExpr();
		bool PrintableExpt();
		bool ArithExpr();
		bool ArithExprMD();
		bool ArithExprASMore();
		bool ArithExprPN();
		bool ArithExprBase();
		bool ArithExprMDMore();
		bool LogicExpr(int,int);
		bool LogicExprAnd();
		bool LogicExprOrMore(int);
		bool LogicExprTF();
		bool LogicExprBase();
		bool LogicExprAndMore(int);
		bool LogicExprComp();

		bool trace;

	public:
		Parser(char*,char*,char*);
		~Parser();
		void parse();
		bool isPass();
};

#endif
