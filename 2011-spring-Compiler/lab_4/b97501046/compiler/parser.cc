/*
   NTU CSIE Compiler 2011 Lab 4
   B97501046 Qing-Cheng Li

   parser.cc
   a parser (LL1) for c-------, using Lab2 to find predict set, modified from parser.cc from Lab3 
 */
#include "parser.h"

Parser::Parser(char * inputFileName, char * outputFileName, char *  programName){

	scanner = new Scanner(inputFileName);
	coGen = new Generator(outputFileName,programName);
	pass = false;
	trace = false;  //default false

	/* Build StringToTokenCode */
	StringToTokenCode["+"] = TC_ADD;
	StringToTokenCode["-"] = TC_SUB;
	StringToTokenCode["*"] = TC_MUT;
	StringToTokenCode["/"] = TC_DIV;
	StringToTokenCode["&&"] = TC_AND;
	StringToTokenCode["||"] = TC_OR;
	StringToTokenCode["!"] = TC_NOT;
	StringToTokenCode["<"] = TC_LT;
	StringToTokenCode[">"] = TC_GT;
	StringToTokenCode["<="] = TC_LE;
	StringToTokenCode[">="] = TC_GE;
	StringToTokenCode["=="] = TC_EQU;
	StringToTokenCode["!="] = TC_NEQ;
	StringToTokenCode["("] = TC_LP;
	StringToTokenCode[")"] = TC_RP;
	StringToTokenCode["["] = TC_LS;
	StringToTokenCode["]"] = TC_RS;
	StringToTokenCode["{"] = TC_LC;
	StringToTokenCode["}"] = TC_RC;
	StringToTokenCode[","] = TC_COM;
	StringToTokenCode[";"] = TC_SEM;
	StringToTokenCode["="] = TC_EQ;
	StringToTokenCode["int"] = TC_INT;
	StringToTokenCode["if"] = TC_IF;
	StringToTokenCode["else"] = TC_ELSE;
	StringToTokenCode["while"] = TC_WHILE;
	StringToTokenCode["break"] = TC_BRK;
	StringToTokenCode["continue"] = TC_CNT;
	StringToTokenCode["scan"] = TC_SCAN;
	StringToTokenCode["print"] = TC_PRINT;
	StringToTokenCode["id"] = TC_ID;
	StringToTokenCode["num"] = TC_NUM;
	StringToTokenCode["string"] = TC_STR;
	StringToTokenCode["println"] = TC_PRINTLN;
}

Parser::~Parser(){

}

void Parser::parse(){   //return AST?Lab4?
	if(!scanner->scan()){
		/* scanner has error, default `pass' is false */
		return;
	}else{
		if(Program())
			pass = true;
		return;
	}
}

bool Parser::isPass(){
	return pass;
}

Token* Parser::match(string symbol,string rule){

	Token* token = scanner->getNextToken();

	if(token==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next input token (`"<< symbol << "'?) ( Rule: " << rule << " )" << endl;
		return NULL;
	}

	if(token->getTokenCode()!=StringToTokenCode[symbol]){
		cerr << "Line " << token->getPosition() << " <Parser> Not `" << symbol << "' ( Rule: " << rule << " )"<< token->getTokenCode() << endl;
		return NULL;
	}

	if(trace)
		cout << "match `" << symbol << "' ( line " << token->getPosition() << " )" << endl; 
	
	return token;
}

/* Recursive Desent Parsing */

/*
   Program -> BlockStmt
*/
bool Parser::Program(){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "<Parser> No input token (maybe the inputfile is NULL)" << endl;
		return false;
	}

	switch(peek->getTokenCode()){
		/* 
		   Program -> BlockStmt
		   Predict:{ 
		*/
		case TC_LC:

			rule = "Program -> BlockStmt";
			if(trace)
				cout << rule << endl;

			coGen->header();

			if(!BlockStmt(0,0)){
				return false;
			}

			coGen->eop();

			return true;
		
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ,no `{' ( Rule: Program -> BlockStmt )" << endl;
			return false;
	}
}

/*
   BlockStmt -> { VarDecl Stmt }
*/
bool Parser::BlockStmt(int c,int b){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next input token ( Rule: BlockStmt -> { VarDecl Stmt } )" << endl;
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   BlockStmt -> { VarDecl Stmt }
	   Predict:{
	*/
		case TC_LC:

			rule = "BlockStmt -> { VarDecl Stmt }";
			if(trace)
				cout << rule << endl;

			if(match("{",rule)==NULL){
				return false;
			}

			coGen->newScope();

			if(!VarDecl()){
				return false;
			}

			if(!Stmt(c,b)){
				return false;
			}

			if(match("}",rule)==NULL){
				return false;
			}

			coGen->endScope();

			return true;
		/* Error */
		default:
			cerr << "<Parser> Syntax error ,no `{' ( Rule: BlockStmt -> { VarDecl Stmt } )" << endl;
			return false;
	}
}

/* 
   VarDecl -> int VarDeclNum id VarDeclId ; VarDecl 
           -> (null)
*/
bool Parser::VarDecl(){

	Token* peek = scanner->peekNextToken();
	Token* id;
	vector<string> d;
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: VarDecl -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   VarDecl -> int VarDeclNum id VarDeclId ; VarDecl
	   Predict:int
	*/
		case TC_INT:   //`int'

			rule =  "VarDecl -> int VarDeclNum id VarDeclId ; VarDecl";
			if(trace)
				cout << rule << endl;

			if(match("int",rule)==NULL){
				return false;
			}

			d = VarDeclNum();
			if(d.size()<0){
				return false;
			}else if(d.size()==1&&d[0]==""){
				return false;
			}

			id = match("id",rule);
			if(id==NULL){
				return false;
			}

			coGen->addId(id->getAttribute(),d.size());
			if(d.size()>0)
				coGen->newArray(coGen->getIndex(id->getAttribute()),d);

			if(!VarDeclId(d)){
				return false;
			}

			if(match(";",rule)==NULL){
				return false;
			}

			if(!VarDecl()){
				return false;
			}

			return true;

	/*
	   VarDecl -> (null)
	   Predict:id break continue if print println scan while { }
	*/
		case TC_LC:
		case TC_RC:
		case TC_IF:
		case TC_WHILE:
		case TC_BRK:
		case TC_CNT:
		case TC_SCAN:
		case TC_PRINT:
		case TC_PRINTLN:
		case TC_ID:
	
			rule = "VarDecl -> (null)";
			if(trace)
				cout << rule << endl;
			
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: VarDecl -> ??? )" << endl;
			return false;
	}
}

/*
   VarDeclNum -> [ num ] VarDeclNum
              -> (null)
*/
vector<string> Parser::VarDeclNum(){

	Token* peek = scanner->peekNextToken();
	string rule;
	Token* num;
	vector<string> d;
	vector<string> r;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: VarDeclNum -> ??? )";
		r.push_back("");
		return r;
	}

	switch(peek->getTokenCode()){
	/*
	   VarDeclNum -> [ num ] VarDeclNum
	   Predict:[  
	*/
		case TC_LS:

			rule = "VarDeclNum -> [ num ] VarDeclNum";
			if(trace)
				cout << rule << endl;

			if(match("[",rule)==NULL){
				r.push_back("");
				return r;
			}

			num = match("num",rule);
			if(num==NULL){
				r.push_back("");
				return r;
			}
			//coGen->ldcInt(num->getAttribute());

			if(match("]",rule)==NULL){
				r.push_back("");
				return r;
			}

			d = VarDeclNum();
			if(d.size()<0){
				r.push_back("");
				return r;
			}else if(d.size()==1&&d[0]==""){
				r.push_back("");
				return r;
			}

			r.push_back(num->getAttribute());
			for(int i=0;i<d.size();i++){
				r.push_back(d[i]);
			}

			return r;
	/*
	   VarDeclNum -> (null)
	   Predict:id
	*/
		case TC_ID:

			rule = "VarDeclNum -> (null) ";
			if(trace)
				cout << rule << endl;
			
			return r;
		
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: VarDeclNum -> ??? )" << endl;
			r.push_back("");
			return r;
	}
}

/*
   VarDeclId -> , id VarDeclId
             -> (null)
*/
bool Parser::VarDeclId(vector<string> d){

	Token* peek = scanner->peekNextToken();
	Token* id;
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: VarDeclId -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   VarDeclId -> , id VarDeclId
	   Predict:,
	*/
		case TC_COM:

			rule = "VarDeclId -> , id VarDeclId";
			if(trace)
				cout << rule << endl;

			if(match(",",rule)==NULL){
				return false;
			}

			id = match("id",rule);
			if(id==NULL){
				return false;
			}
			coGen->addId(id->getAttribute(),d.size());
			if(d.size()>0)
				coGen->newArray(coGen->getIndex(id->getAttribute()),d);

			if(!VarDeclId(d)){
				return false;
			}

			return true;
	/*
	   VarDeclId -> (null)
	   Predict:;
	*/
		case TC_SEM:
		
			rule = "VarDeclId -> (null)";
			if(trace)
				cout << rule << endl;
	
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: VarDeclId -> ??? )" << endl;
			return false;
	}
}

/*
   Var -> id VarArithExpr
*/
string Parser::Var(int get){

	Token* peek = scanner->peekNextToken();
	string rule;
	Token* id;
	int index;
	int d;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: Var -> id VarArithExpr )";
		return "";
	}

	switch(peek->getTokenCode()){
	/*
	   Var -> id VarArithExpr
	   Predict:id
	*/
		case TC_ID:

			rule = "Var -> id VarArithExpr";
			if(trace)
				cout << rule << endl;

			id = match("id",rule);
			if(id==NULL){
				return "";
			}
			index = coGen->getIndex(id->getAttribute());
			d = coGen->getDim(id->getAttribute());

			if(!VarArithExpr(index)){
				return "";
			}

			if(get==1){
				if(d==0)
					coGen->iload(index);
				else
					coGen->iaload();
			}

			return id->getAttribute();
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: Var -> id VarArithExpr )" << endl;
			return "";
	}
}

/*
   VarArithExpr -> [ ArithExpr ]   VarArithExpr
                -> (null)
*/
bool Parser::VarArithExpr(int index){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: VarArithExpr -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   VarArithExpr -> [ ArithExpr ]   VarArithExpr
	   Predict:[
	*/
		case TC_LS:

			rule = "VarArithExpr -> [ ArithExpr ]   VarArithExpr" ;
			if(trace)
				cout << rule << endl;

			if(match("[",rule)==NULL){
				return false;
			}

			if(index<0){
				coGen->aaload();
			}else{
				coGen->aload(index);
				index = -1;
			}

			if(!ArithExpr()){
				return false;
			}

			if(match("]",rule)==NULL){
				return false;
			}

			if(!VarArithExpr(index)){
				return false;
			}

			return true;
	/*
	   VarArithExpr -> (null)
	   Predict:!= && ) * + , - / ; < <= = == > >= ] ||
	*/
		case TC_NEQ:
		case TC_AND:
		case TC_RP:
		case TC_MUT:
		case TC_ADD:
		case TC_COM:
		case TC_SUB:
		case TC_DIV:
		case TC_SEM:
		case TC_LT:
		case TC_LE:
		case TC_EQ:
		case TC_EQU:
		case TC_GT:
		case TC_GE:
		case TC_RS:
		case TC_OR:
	
			rule = "VarArithExpr -> (null)" ;
			if(trace)
				cout << rule << endl;
		
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: VarArithExpr -> ??? )" << endl;
			return false;
	}
}

/*
   Stmt -> Var = ArithExpr ; Stmt
	    -> if ( LogicExpr ) BlockStmt StmtElse Stmt
        -> while ( LogicExpr ) BlockStmt Stmt
        -> break ; Stmt
        -> continue ; Stmt
        -> scan ( Var StmtVar ) ; Stmt
        -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt
        -> println ( PrintableExpt StmtPrintableExpr ) ; Stmt
        -> BlockStmt Stmt
        -> (null)
*/
bool Parser::Stmt(int c,int b){

	Token* peek = scanner->peekNextToken();
	string rule;
	string var;
	int index;
	int d;
	int s,t,f,a;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: Stmt -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   Stmt -> Var = ArithExpr ; Stmt
	   Predict:id
	*/
		case TC_ID:

			rule = "Stmt -> Var = ArithExpr ; Stmt" ;
			if(trace)
				cout << rule << endl;

			var = Var(0);
			if(var==""){
				return false;
			}

			if(match("=",rule)==NULL){
				return false;
			}

			if(!ArithExpr()){
				return false;
			}

			index = coGen->getIndex(var);
			d = coGen->getDim(var);
			if(d==0)
				coGen->istore(index);
			else
				coGen->iastore();

			if(match(";",rule)==NULL){
				return false;
			}

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> if ( LogicExpr ) BlockStmt StmtElse Stmt
	   Predict:if
	*/
		case TC_IF:

			rule = "Stmt -> if ( LogicExpr ) BlockStmt StmtElse Stmt" ;
			if(trace)
				cout << rule << endl;

			if(match("if",rule)==NULL){
				return false;
			}

			if(match("(",rule)==NULL){
				return false;
			}

			t = coGen->getNewLabel();
			f = coGen->getNewLabel();
			a = coGen->getNewLabel();

			if(!LogicExpr(t,f)){
				return false;
			}

			if(match(")",rule)==NULL){
				return false;
			}

			coGen->putLabel(t);

			if(!BlockStmt(c,b)){
				return false;
			}

			coGen->jump(a);
			coGen->putLabel(f);

			if(!StmtElse(c,b)){
				return false;
			}

			coGen->putLabel(a);

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> while ( LogicExpr ) BlockStmt Stmt
	   Predict:while
	*/
		case TC_WHILE:

			rule = "Stmt -> while ( LogicExpr ) BlockStmt Stmt" ;
			if(trace)
				cout << rule << endl;

			if(match("while",rule)==NULL){
				return false;
			}

			s = coGen->getNewLabel();
			t = coGen->getNewLabel();
			f = coGen->getNewLabel();
			coGen->putLabel(s);

			if(match("(",rule)==NULL){
				return false;
			}

			if(!LogicExpr(t,f)){
				return false;
			}

			if(match(")",rule)==NULL){
				return false;
			}

			coGen->putLabel(t);

			if(!BlockStmt(s,f)){
				return false;
			}

			coGen->jump(s);
			coGen->putLabel(f);

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> break ; Stmt
	   Predict:break
	*/
		case TC_BRK:

			rule =  "Stmt -> break ; Stmt" ;
			if(trace)
				cout << "Stmt -> break ; Stmt" << endl;

			if(match("break",rule)==NULL){
				return false;
			}

			coGen->jump(b);

			if(match(";",rule)==NULL){
				return false;
			}

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> continue ; Stmt
	   Predict:continue
	*/
		case TC_CNT:

			rule = "Stmt -> continue ; Stmt";
			if(trace)
				cout << rule << endl;

			if(match("continue",rule)==NULL){
				return false;
			}

			coGen->jump(c);

			if(match(";",rule)==NULL){
				return false;
			}

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> scan ( Var StmtVar ) ; Stmt
	   Predict:scan
	*/
		case TC_SCAN:

			rule = "Stmt -> scan ( Var StmtVar ) ; Stmt" ;
			if(trace)
				cout << rule << endl;

			if(match("scan",rule)==NULL){
				return false;
			}

			if(match("(",rule)==NULL){
				return false;
			}


			var = Var(0);
			if(var==""){
				return false;
			}

			coGen->scan();
		
			index = coGen->getIndex(var);
			d = coGen->getDim(var);

			if(d==0){
				coGen->istore(index);
			}else{
				coGen->iastore();
			}

			if(!StmtVar()){
				return false;
			}

			if(match(")",rule)==NULL){
				return false;
			}

			if(match(";",rule)==NULL){
				return false;
			}

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt
	   Predict:print
	*/
		case TC_PRINT:

			rule = "Stmt -> print ( PrintableExpt StmtPrintableExpr ) ; Stmt";
			if(trace)
				cout << rule << endl;

			if(match("print",rule)==NULL){
				return false;
			}

			if(match("(",rule)==NULL){
				return false;
			}

			if(!PrintableExpt()){
				return false;
			}

			if(!StmtPrintableExpr()){
				return false;
			}

			if(match(")",rule)==NULL){
				return false;
			}

			if(match(";",rule)==NULL){
				return false;
			}

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> println ( PrintableExpt StmtPrintableExpr ) ; Stmt
	   Predict:println
	*/
		case TC_PRINTLN:
	
			rule = "Stmt -> println ( PrintableExpt StmtPrintableExpr ) ; Stmt" ;
			if(trace)
				cout << rule << endl;

			if(match("println",rule)==NULL){
				return false;
			}

			if(match("(",rule)==NULL){
				return false;
			}

			if(!PrintableExpt()){
				return false;
			}

			if(!StmtPrintableExpr()){
				return false;
			}

			if(match(")",rule)==NULL){
				return false;
			}

			if(match(";",rule)==NULL){
				return false;
			}

			coGen->println();

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> BlockStmt Stmt
	   Predict:{
	*/
		case TC_LC:

			rule = "Stmt -> BlockStmt Stmt";
			if(trace)
				cout << rule << endl;

			if(!BlockStmt(c,b)){
				return false;
			}

			if(!Stmt(c,b)){
				return false;
			}

			return true;
	/*
	   Stmt -> (null)
	   Predict:}
	*/
		case TC_RC:
	
			rule = "Stmt -> (null)";
			if(trace)
				cout << rule << endl;
			
			return true;
		
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: Stmt -> ??? )" << endl;
			return false;
	}
}

/*
   StmtElse -> else BlockStmt
            -> (null)
*/
bool Parser::StmtElse(int c,int b){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: StmtElse -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   StmtElse -> else BlockStmt
	   Predict:else
	*/
		case TC_ELSE:

			rule = "StmtElse -> else BlockStmt" ;
			if(trace)
				cout << rule << endl;

			if(match("else",rule)==NULL){
				return false;
			}

			if(!BlockStmt(c,b)){
				return false;
			}

			return true;
	/*
	   StmtElse -> (null)
	   Predict:id break continue if print println scan while { }
	*/
		case TC_ID:
		case TC_BRK:
		case TC_CNT:
		case TC_IF:
		case TC_PRINT:
		case TC_PRINTLN:
		case TC_SCAN:
		case TC_WHILE:
		case TC_LC:
		case TC_RC:
	
			rule = "StmtElse -> (null)";
			if(trace)
				cout << rule << endl;
					
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: StmtElse -> ??? )" << endl;
			return false;
	}
}

/*
   StmtVar -> , Var StmtVar
           -> (null)
*/
bool Parser::StmtVar(){

	Token* peek = scanner->peekNextToken();
	string rule ;
	string var;
	int index;
	int d;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: StmtVar -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   StmtVar -> , Var StmtVar
	   Predict:,
	*/
		case TC_COM:

			rule =  "StmtVar -> , Var StmtVar";
			if(trace)
				cout << "StmtVar -> , Var StmtVar" << endl;

			if(match(",",rule)==NULL){
				return false;
			}
			
			var = Var(0);
			if(var==""){
				return false;	
			}

			coGen->scan();

			index = coGen->getIndex(var);
			d = coGen->getDim(var);

			if(d==0)
				coGen->istore(index);
			else
				coGen->iastore();

			if(!StmtVar()){
				return false;
			}

			return true;
	/*
	   StmtVar -> (null)
	   Predict:)
	*/
		case TC_RP:
	
			rule =  "StmtVar -> (null)";
			if(trace)
				cout << "StmtVar -> (null)" << endl;
					
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: StmtVar -> ??? )" << endl;
			return false;
	}
}

/*
   StmtPrintableExpr -> , PrintableExpt StmtPrintableExpr
                     -> (null)
*/
bool Parser::StmtPrintableExpr(){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: StmtPrintableExpr -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   StmtPrintableExpr -> , PrintableExpt StmtPrintableExpr
       Predict:,
	*/
		case TC_COM:

			rule = "StmtPrintableExpr -> , PrintableExpt StmtPrintableExpr";
			if(trace)
				cout << rule << endl;

			if(match(",",rule)==NULL){
				return false;
			}

			if(!PrintableExpt()){
				return false;
			}

			if(!StmtPrintableExpr()){
				return false;
			}

			return true;
	/*
	   StmtPrintableExpr -> (null)
	   Predict:)
	*/
		case TC_RP:
	
			rule = "StmtPrintableExpr -> (null)";
			if(trace)
				cout << "StmtPrintableExpr -> (null)" << endl;
			
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: StmtPrintableExpr -> ??? )" << endl;
			return false;
	}
}

/*
   PrintableExpt -> string
                 -> ArithExpr
*/
bool Parser::PrintableExpt(){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: PrintableExpt -> ??? )";
		return false;
	}

	Token* str;

	switch(peek->getTokenCode()){
	/*
	   PrintableExpt -> string
	   Predict:string
	*/
		case TC_STR:

			rule =  "PrintableExpt -> string";
			if(trace)
				cout << rule << endl;

			str = match("string",rule);
			if(str==NULL){
				return false;
			}

			coGen->ldc(str->getAttribute());
			coGen->printString();

			return true;
	/*
	   PrintableExpt -> ArithExpr
	   Predict:id num ( + -
	*/
		case TC_ID:
		case TC_NUM:
		case TC_LP:
		case TC_ADD:
		case TC_SUB:

			rule = "PrintableExpt -> ArithExpr";
			if(trace)
				cout << rule << endl;

			if(!ArithExpr()){
				return false;
			}

			coGen->printInt();

			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: PrintableExpt -> ??? )" << endl;
			return false;
	}
}

/*
   ArithExpr -> ArithExprMD ArithExprASMore
*/
bool Parser::ArithExpr(){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExpr -> ArithExprMD ArithExprASMore )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   ArithExpr -> ArithExprMD ArithExprASMore
	   Predict:id num ( + -
	*/
		case TC_ID:
		case TC_NUM:
		case TC_LP:
		case TC_ADD:
		case TC_SUB:

			rule = "ArithExpr -> ArithExprMD ArithExprASMore" ;
			if(trace)
				cout << "ArithExpr -> ArithExprMD ArithExprASMore" << endl;

			if(!ArithExprMD()){
				return false;
			}

			if(!ArithExprASMore()){
				return false;
			}

			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExpr -> ArithExprMD ArithExprASMore )" << endl;
			return false;
	}
}

/*
   ArithExprMD -> ArithExprPN ArithExprMDMore
*/
bool Parser::ArithExprMD(){

	Token* peek = scanner->peekNextToken();
	string rule ;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprMD -> ArithExprPN ArithExprMDMore )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   ArithExprMD -> ArithExprPN ArithExprMDMore
	   Predict:id num ( + -
	*/
		case TC_ID:
		case TC_NUM:
		case TC_LP:
		case TC_ADD:
		case TC_SUB:

			rule =  "ArithExprMD -> ArithExprPN ArithExprMDMore" ;
			if(trace)
				cout << rule << endl;

			if(!ArithExprPN()){
				return false;
			}

			if(!ArithExprMDMore()){
				return false;
			}

			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprMD -> ArithExprPN ArithExprMDMore )" << endl;
			return false;
	}
}

/*
   ArithExprASMore -> + ArithExprMD ArithExprASMore
                   -> - ArithExprMD ArithExprASMore
		           -> (null)
*/
bool Parser::ArithExprASMore(){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprASMore -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   ArithExprASMore -> + ArithExprMD ArithExprASMore
	   Predict:+
	*/
		case TC_ADD:

			rule = "ArithExprASMore -> + ArithExprMD ArithExprASMore";
			if(trace)
				cout << rule << endl;

			if(match("+",rule)==NULL){
				return false;
			}

			if(!ArithExprMD()){
				return false;
			}

			coGen->iadd();

			if(!ArithExprASMore()){
				return false;
			}

			return true;
	/*
	   ArithExprASMore -> - ArithExprMD ArithExprASMore
	   Predict:-
	*/
		case TC_SUB:

			rule = "ArithExprASMore -> - ArithExprMD ArithExprASMore" ;
			if(trace)
				cout << "ArithExprASMore -> - ArithExprMD ArithExprASMore" << endl;

			if(match("-",rule)==NULL){
				return false;
			}

			if(!ArithExprMD()){
				return false;
			}

			coGen->isub();

			if(!ArithExprASMore()){
				return false;
			}

			return true;
	/*
	   ArithExprASMore -> (null)
	   Predict:!= && ) , ; < <= == > >= ] ||
	*/
		case TC_NEQ:
		case TC_AND:
		case TC_RP:
		case TC_COM:
		case TC_SEM:
		case TC_LT:
		case TC_LE:
		case TC_EQU:
		case TC_GT:
		case TC_GE:
		case TC_RS:
		case TC_OR:
		
			rule = "ArithExprASMore -> (null)";
			if(trace)
				cout << rule << endl;
					
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprASMore -> ??? )" << endl;
			return false;
	}
}

/*
   ArithExprPN -> + ArithExprBase
               -> - ArithExprBase
               -> ArithExprBase
*/
bool Parser::ArithExprPN(){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprPN -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   ArithExprPN -> + ArithExprBase
	   Predict: +
	*/
		case TC_ADD:

			rule = "ArithExprPN -> + ArithExprBase";
			if(trace)
				cout << rule << endl;

			if(match("+",rule)==NULL){
				return false;
			}

			if(!ArithExprBase()){
				return false;
			}

			return true;        
	/*
	   ArithExprPN -> - ArithExprBase
	   Predict: -
	*/
		case TC_SUB:

			rule = "ArithExprPN -> - ArithExprBase" ;
			if(trace)
				cout << rule << endl;

			if(match("-",rule)==NULL){
				return false;
			}

			if(!ArithExprBase()){
				return false;
			}

			coGen->ineg();

			return true;   
	/*
	   ArithExprPN -> ArithExprBase
	   Predict: id num (
	*/
		case TC_ID:
		case TC_NUM:
		case TC_LP:

			rule =  "ArithExprPN -> ArithExprBase";
			if(trace)
				cout << rule << endl;

			if(!ArithExprBase()){
				return false;
			}

			return true; 
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprPN -> ??? )" << endl;
			return false;
	}
}

/*
   ArithExprBase -> num
                 -> Var
                 -> ( ArithExpr )
*/
bool Parser::ArithExprBase(){

	Token* peek = scanner->peekNextToken();
	Token* num;
	string rule;
	string var;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprBase -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   ArithExprBase -> num
	   Predict:num
	*/
		case TC_NUM:

			rule = "ArithExprBase -> num" ;
			if(trace)
				cout << rule << endl;

			num = match("num",rule);
			if(num==NULL){
				return false;
			}

			coGen->ldcInt(num->getAttribute());

			return true;
	/*
	   ArithExprBase -> Var
	   Predict:id
	*/
		case TC_ID:

			rule = "ArithExprBase -> Var";
			if(trace)
				cout << rule << endl;

			var = Var(1);
			if(var==""){
				return false;
			}

			return true;
	/*
	   ArithExprBase -> ( ArithExpr )
	   Predict:(
	*/
		case TC_LP:

			rule = "ArithExprBase -> ( ArithExpr )";
			if(trace)
				cout << rule << endl;

			if(match("(",rule)==NULL){
				return false;
			}

			if(!ArithExpr()){
				return false;
			}

			if(match(")",rule)==NULL){
				return false;
			}

			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprBase -> ??? )" << endl;
			return false;
	}
}

/*
   ArithExprMDMore -> * ArithExprPN ArithExprMDMore
                   -> / ArithExprPN ArithExprMDMore
                   -> (null)
*/
bool Parser::ArithExprMDMore(){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: ArithExprMDMore -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   ArithExprMDMore -> * ArithExprPN ArithExprMDMore
	   Predict:*
	*/
		case TC_MUT:
			
			rule = "ArithExprMDMore -> * ArithExprPN ArithExprMDMore";
			if(trace)
				cout << rule << endl;

			if(match("*",rule)==NULL){
				return false;
			}

			if(!ArithExprPN()){
				return false;
			}

			coGen->imul();

			if(!ArithExprMDMore()){
				return false;
			}

			return true;
	/*
	   ArithExprMDMore -> / ArithExprPN ArithExprMDMore
	   Predict:/
	*/
		case TC_DIV:

			rule =  "ArithExprMDMore -> / ArithExprPN ArithExprMDMore";
			if(trace)
				cout << rule << endl;
			
			if(match("/",rule)==NULL){
				return false;
			}

			if(!ArithExprPN()){
				return false;
			}

			coGen->idiv();

			if(!ArithExprMDMore()){
				return false;
			}

			return true;
	/*
	   ArithExprMDMore -> (null)
	   Predict:!= && ) + , - ; < <= == > >= ] ||
	*/
		case TC_NEQ:
		case TC_AND:
		case TC_RP:
		case TC_ADD:
		case TC_COM:
		case TC_SUB:
		case TC_SEM:
		case TC_LT:
		case TC_LE:
		case TC_EQU:
		case TC_GT:
		case TC_GE:
		case TC_RS:
		case TC_OR:
	
			rule = "ArithExprMDMore -> (null)";
			if(trace)
				cout << rule << endl;
			
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: ArithExprMDMore -> ??? )" << endl;
			return false;
	}
}

/*
   LogicExpr -> LogicExprAnd LogicExprOrMore
*/
bool Parser::LogicExpr(int t,int f){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: LogicExpr -> LogicExprAnd LogicExprOrMore )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   LogicExpr -> LogicExprAnd LogicExprOrMore
	   Predict:id num ! ( + - [
	*/
		case TC_ID:
		case TC_NUM:
		case TC_NOT:
		case TC_LP:
		case TC_ADD:
		case TC_SUB:
		case TC_LS:

			rule =  "LogicExpr -> LogicExprAnd LogicExprOrMore" ;
			if(trace)
				cout << rule << endl;

			if(!LogicExprAnd()){
				return false;
			}

			coGen->ifne(t);

			if(!LogicExprOrMore(t)){
				return false;
			}

			coGen->jump(f);

			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExpr -> LogicExprAnd LogicExprOrMore )" << endl;
			return false;
	}
}

/*
   LogicExprAnd -> LogicExprTF LogicExprAndMore
*/
bool Parser::LogicExprAnd(){

	Token* peek = scanner->peekNextToken();
	string rule;
	int f,af;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: LogicExprAnd -> LogicExprTF LogicExprAndMore )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   LogicExprAnd -> LogicExprBase LogicExprAndMore
	   Predict:id num ! ( + - [
	*/
		case TC_ID:
		case TC_NUM:
		case TC_NOT:
		case TC_LP:
		case TC_ADD:
		case TC_SUB:
		case TC_LS:

			rule =  "LogicExprAnd -> LogicExprTF LogicExprAndMore";
			if(trace)
				cout << rule << endl;

			if(!LogicExprTF()){
				return false;
			}

			f = coGen->getNewLabel();
			af = coGen->getNewLabel();
			coGen->ifeq(f);

			if(!LogicExprAndMore(f)){
				return false;
			}

			coGen->ldcInt("1");
			coGen->jump(af);
			coGen->putLabel(f);
			coGen->ldcInt("0");
			coGen->putLabel(af);

			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprAnd -> LogicExprTF LogicExprAndMore )" << endl;
			return false;
	}
}

/*
   LogicExprOrMore -> || LogicExprAnd LogicExprOrMore
                   -> (null)
*/
bool Parser::LogicExprOrMore(int t){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: LogicExprOrMore -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   LogicExprOrMore -> || LogicExprAnd LogicExprOrMore
	   Predict:||
	*/
		case TC_OR:

			rule =  "LogicExprOrMore -> || LogicExprAnd LogicExprOrMore" ;
			if(trace)
				cout << rule << endl;

			if(match("||",rule)==NULL){
				return false;
			}

			if(!LogicExprAnd()){
				return false;
			}

			coGen->ifne(t);

			if(!LogicExprOrMore(t)){
				return false;
			}

			return true;
	/*
	   LogicExprOrMore -> (null)
	   Predict:) ]
	*/
		case TC_RP:
		case TC_RS:
	
			rule = "LogicExprOrMore -> (null)";
			if(trace)
				cout << rule << endl;
			
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprOrMore -> ??? )" << endl;
			return false;
	}
}

/*
   LogicExprTF -> ! LogicExprBase
               -> LogicExprBase
*/
bool Parser::LogicExprTF(){

	Token* peek = scanner->peekNextToken();
	string rule ;
	int t,af;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule:LogicExprTF -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   LogicExprTF -> ! LogicExprBase
	   Predict:!
	*/
		case TC_NOT:

			rule =  "LogicExprTF -> ! LogicExprBase";
			if(trace)
				cout << rule << endl;

			if(match("!",rule)==NULL){
				return false;
			}

			if(!LogicExprBase()){
				return false;
			}

			t = coGen->getNewLabel();
			af = coGen->getNewLabel();
			coGen->ifeq(t);
			coGen->ldcInt("0");
			coGen->jump(af);
			coGen->putLabel(t);
			coGen->ldcInt("1");
			coGen->putLabel(af);

			return true;
	/*
	   LogicExprTF -> LogicExprBase
	   Predict: id num ( + - [
	*/
		case TC_ID:
		case TC_NUM:
		case TC_LP:
		case TC_ADD:
		case TC_SUB:
		case TC_LS:

			rule = "LogicExprTF -> LogicExprBase" ;
			if(trace)
				cout << rule << endl;

			if(!LogicExprBase()){
				return false;
			}
			
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprTF -> ??? )" << endl;
			return false;
	}
}

/*
   LogicExprBase -> ArithExpr LogicExprComp
                 -> [ LogicExpr ]
*/
bool Parser::LogicExprBase(){

	Token* peek = scanner->peekNextToken();
	string rule;
	int t,f,a;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << "<Parser> No next token, cannot predict rule! ( Rule: LogicExprBase -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   LogicExprBase -> ArithExpr LogicExprComp
	   Predict:id num ( + -
	*/
		case TC_ID:
		case TC_NUM:
		case TC_LP:
		case TC_ADD:
		case TC_SUB:

			rule =  "LogicExprBase -> ArithExpr LogicExprComp" ;
			if(trace)
				cout << rule << endl;

			if(!ArithExpr()){
				return false;
			}

			if(!LogicExprComp()){
				return false;
			}

			return true;
	/*
	   LogicExprBase -> [ LogicExpr ]
	   Predict:[
	*/
		case TC_LS:

			rule =  "LogicExprBase -> [ LogicExpr ]" ;
			if(trace)
				cout << rule << endl;

			if(match("[",rule)==NULL){
				return false;
			}

			t = coGen->getNewLabel();
			f = coGen->getNewLabel();
			a = coGen->getNewLabel();

			if(!LogicExpr(t,f)){
				return false;
			}

			coGen->putLabel(t);
			coGen->ldcInt("1");
			coGen->jump(a);
			coGen->putLabel(f);
			coGen->ldcInt("0");
			coGen->putLabel(a);

			if(match("]",rule)==NULL){
				return false;
			}

			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprBase -> ??? )" << endl;
			return false;
	}
}

/*
   LogicExprAndMore -> && LogicExprTF LogicExprAndMore
                    -> (null)
*/
bool Parser::LogicExprAndMore(int f){

	Token* peek = scanner->peekNextToken();
	string rule;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() << " <Parser> No next token, cannot predict rule! ( Rule: LogicExprAndMore -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   LogicExprAndMore -> && LogicExprTF LogicExprAndMore
	   Predict:&&
	*/
		case TC_AND:

			rule = "LogicExprAndMore -> && LogicExprTF LogicExprAndMore";
			if(trace)
				cout << rule << endl;

			if(match("&&",rule)==NULL){
				return false;
			}

			if(!LogicExprTF()){
				return false;
			}

			coGen->ifeq(f);

			if(!LogicExprAndMore(f)){
				return false;
			}

			return true;
	/*
	   LogicExprAndMore -> (null)
	   Predict:) ] ||
	*/
		case TC_RP:
		case TC_RS:
		case TC_OR:
	
			rule = "LogicExprAndMore -> (null)" ;
			if(trace)
				cout << rule << endl;
			
			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprAndMore -> ??? )" << endl;
			return false;
	}
}

/*
   LogicExprComp -> > ArithExpr
                 -> >= ArithExpr
                 -> < ArithExpr
                 -> <= ArithExpr
                 -> == ArithExpr
				 -> != ArithExpr
*/
bool Parser::LogicExprComp(){

	Token* peek = scanner->peekNextToken();
	string rule;
	int t;
	int af;
	if(peek==NULL){
		cerr << "Near line " << scanner->getPreviousToken()->getPosition() <<" <Parser> No next token, cannot predict rule! ( Rule: LogicExprComp -> ??? )";
		return false;
	}

	switch(peek->getTokenCode()){
	/*
	   LogicExprComp -> > ArithExpr
	   Predict:>
	*/
		case TC_GT:
		
			rule = "LogicExprComp -> > ArithExpr";
			if(trace)
				cout << rule << endl;

			if(match(">",rule)==NULL){
				return false;
			}

			if(!ArithExpr()){
				return false;
			}

			t = coGen->getNewLabel();
			af = coGen->getNewLabel();
			coGen->if_icmpgt(t);
			coGen->ldcInt("0");
			coGen->jump(af);
			coGen->putLabel(t);
			coGen->ldcInt("1");
			coGen->putLabel(af);

			return true;
	/*
	   LogicExprComp -> >= ArithExpr
	   Predict:>=
	*/
		case TC_GE:

			rule = "LogicExprComp -> >= ArithExpr";
			if(trace)
				cout << rule << endl;

			if(match(">=",rule)==NULL){
				return false;
			}

			if(!ArithExpr()){
				return false;
			}

			t = coGen->getNewLabel();
			af = coGen->getNewLabel();
			coGen->if_icmpge(t);
			coGen->ldcInt("0");
			coGen->jump(af);
			coGen->putLabel(t);
			coGen->ldcInt("1");
			coGen->putLabel(af);

			return true;
	/*
	   LogicExprComp -> < ArithExpr
	   Predict:<
	*/
		case TC_LT:

			rule = "LogicExprComp -> < ArithExpr";
			if(trace)
				cout << rule << endl;

			if(match("<",rule)==NULL){
				return false;
			}

			if(!ArithExpr()){
				return false;
			}

			t = coGen->getNewLabel();
			af = coGen->getNewLabel();
			coGen->if_icmplt(t);
			coGen->ldcInt("0");
			coGen->jump(af);
			coGen->putLabel(t);
			coGen->ldcInt("1");
			coGen->putLabel(af);

			return true;
	/*
	   LogicExprComp -> <= ArithExpr
	   Predict:<=
	*/
		case TC_LE:

			rule =  "LogicExprComp -> <= ArithExpr" ;
			if(trace)
				cout << rule << endl;

			if(match("<=",rule)==NULL){
				return false;
			}

			if(!ArithExpr()){
				return false;
			}

			t = coGen->getNewLabel();
			af = coGen->getNewLabel();
			coGen->if_icmple(t);
			coGen->ldcInt("0");
			coGen->jump(af);
			coGen->putLabel(t);
			coGen->ldcInt("1");
			coGen->putLabel(af);

			return true;
	/*
	   LogicExprComp -> == ArithExpr
	   Predict:==
	*/
		case TC_EQU:

			rule =  "LogicExprComp -> == ArithExpr";
			if(trace)
				cout << rule << endl;

			if(match("==",rule)==NULL){
				return false;
			}

			if(!ArithExpr()){
				return false;
			}

			t = coGen->getNewLabel();
			af = coGen->getNewLabel();
			coGen->if_icmpeq(t);
			coGen->ldcInt("0");
			coGen->jump(af);
			coGen->putLabel(t);
			coGen->ldcInt("1");
			coGen->putLabel(af);

			return true;
	/*
	   LogicExprComp -> != ArithExpr
	   Predict:!=
	*/
		case TC_NEQ:

			rule = "LogicExprComp -> != ArithExpr" ;
			if(trace)
				cout << rule << endl;

			if(match("!=",rule)==NULL){
				return false;
			}

			if(!ArithExpr()){
				return false;
			}

			t = coGen->getNewLabel();
			af = coGen->getNewLabel();
			coGen->if_icmpne(t);
			coGen->ldcInt("0");
			coGen->jump(af);
			coGen->putLabel(t);
			coGen->ldcInt("1");
			coGen->putLabel(af);

			return true;
		/* Error */
		default:
			cerr << "Line " << peek->getPosition() << " <Parser> Syntax error ( Rule: LogicExprComp -> ??? )" << endl;
			return false;
	}
}

