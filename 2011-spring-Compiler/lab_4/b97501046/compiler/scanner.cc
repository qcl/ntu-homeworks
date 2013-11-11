/*
   NTU CSIE Compiler 2011 Lab 4
   B97501046 Qing-Cheng Li

   scanner.cc
   The scanner of cmm (modified from lab3)
 */
#include "scanner.h"
#include <string>
#include <iostream>

using namespace std;

Scanner::Scanner(char * inputFileName){
	fin.open(inputFileName);
	currentLineNum = 1;
	currentTokenIndex = 0;
	tokenString.clear();
	error = false;
}

Scanner::~Scanner(){
	fin.close();
}

Token* Scanner::scanNextToken(){
	if(!fin.eof()){
		Token* token;
		string str;     //used to record attribute
		char num[11];   //used to record number
		int n;
		char c = fin.get();
		switch(c){
			case '+':
				token = new Token(43,currentLineNum);
				break;
			case '-':
				token = new Token(45,currentLineNum);
				break;
			case '*':
				token = new Token(42,currentLineNum);
				break;
			case '/':
				c = fin.get();
				if(c!='*'){
					token = new Token(47,currentLineNum);
					fin.unget();
				}else{
					//Deal with comment
					//Start of comment
					while(true){
						c = fin.get();
						if(c=='*'){
							c = fin.get();
							if(c=='/'){
								//End of comment
								token = scanNextToken(); //Get Next Token then return it.
								break;
							}else{
								fin.unget();
							}
						}else if(c=='\n'){
							currentLineNum++;
						}else if(c==EOF){
							//ERROR
							//return a null token and output error msg
							cerr << "Line " << currentLineNum << " <Scanner> Comment cannot find */" << endl;
							error = true;
							token = NULL;
							break;
						}
					}
				}
				break;
			case '&':
				c = fin.get();
				if(c=='&'){
					token = new Token(256,currentLineNum);
				}else{
					//ERROR
					//return a null token and output error msg
					cerr << "Line " << currentLineNum << " <Scanner> & must follow by another & like &&" << endl;
					error = true;
					token = NULL;
					fin.unget();
				}
				break;
			case '|':
				c = fin.get();
				if(c=='|'){
					token = new Token(257,currentLineNum);
				}else{
					//ERROR
					//return a null token and output error msg
					cerr << "Line " << currentLineNum << " <Scanner> | must follow by another | like ||" << endl;
					error = true;
					token = NULL;
					fin.unget();
				}
				break;
			case '!':
				c = fin.get();
				if(c=='='){
					token = new Token(261,currentLineNum);
				}else{
					token = new Token(33,currentLineNum);
					fin.unget();
				}
				break;
			case '<':
				c = fin.get();
				if(c=='='){
					token = new Token(258,currentLineNum);
				}else{
					token = new Token(60,currentLineNum);
					fin.unget();
				}
				break;
			case '>':
				c = fin.get();
				if(c=='='){
					token = new Token(259,currentLineNum);
				}else{
					token = new Token(62,currentLineNum);
					fin.unget();
				}
				break;
			case '=':
				c = fin.get();
				if(c=='='){
					token = new Token(260,currentLineNum);
				}else{
					token = new Token(61,currentLineNum);
					fin.unget();
				}
				break;
			case '(':
				token = new Token(40,currentLineNum);
				break;
			case ')':
				token = new Token(41,currentLineNum);
				break;
			case '[':
				token = new Token(91,currentLineNum);
				break;
			case ']':
				token = new Token(93,currentLineNum);
				break;
			case '{':
				token = new Token(123,currentLineNum);
				break;
			case '}':
				token = new Token(125,currentLineNum);
				break;
			case ',':
				token = new Token(44,currentLineNum);
				break;
			case ';':
				token = new Token(59,currentLineNum);
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				//Number ( 0~999999)
				n = c - '0';
				while(true){
					c = fin.get();
					if(c>='0'&&c<='9'){
						n = n*10 + (c-'0');
					}else{
						fin.unget();
						break;
					}
				}
				if(n>999999){
					//error = true;
					cerr << "Line " << currentLineNum << " <Scanner> Number must less than 999999" << endl;
				}
				sprintf(num,"0x%08x\0",n);
				str = string(num);
				token = new Token(271,currentLineNum,str);
				str = "";   //clear str
				break;
			case 0x0a:  // `\n'
				currentLineNum++;
			case 0x20:
			case 0x09:
			case 0x0d:
				token = scanNextToken();
				break;
			case '"':
				str="";
				while(true){
					c = fin.get();
					if(c>=' '&&c<='~'){   //printable
						if(c==0x5c){    // `\'
							c = fin.get();
							if(c==0x5c){    // `\'
								str.append<int>(1,0x5c);    // `\'
							}else if(c=='"'){
								str.append(1,'"');
							}else if(c==EOF){
								break;
							}else{
								fin.unget();
							}
						}else if(c=='"'){
							break;
						}else{
							str.append(1,c);
						}
					}else if(c==EOF){
						//ERROR
						error = true;
						cerr << "Line" << currentLineNum << " <Scanner> String cannot find anothrt \"" << endl;
						break;
					}else{
						//ERROR
						//recovery: don't add those unprintabl character to str
						//error = true;
						cerr << "Line" << currentLineNum << " <Scanner> String contant unprintable characters" << endl;
					}
				}
				token = new Token(272,currentLineNum,str);
				str = "";
				break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
				//Identifier or Keyword
				str="";
				str.append(1,c);
				while(true){
					c = fin.get();
					if((c>='0'&&c<='9') || (c>='a'&&c<='z') || (c>='A'&&c<='Z') || c=='_' ){
						str.append(1,c);
					}else{
						fin.unget();
						break;
					}
				}
				switch(str.length()){
					case 2:
						if(str.compare("if")==0){
							token = new Token(263,currentLineNum);
						}else{
							token = new Token(270,currentLineNum,str);
						}
						break;
					case 3:
						if(str.compare("int")==0){
							token = new Token(262,currentLineNum);
						}else{
							token = new Token(270,currentLineNum,str);
						}
						break;
					case 4:
						if(str.compare("else")==0){
							token = new Token(264,currentLineNum);
						}else if(str.compare("scan")==0){
							token = new Token(268,currentLineNum);
						}else{
							token = new Token(270,currentLineNum,str);
						}
						break;
					case 5:
						if(str.compare("while")==0){
							token = new Token(265,currentLineNum);
						}else if(str.compare("break")==0){
							token = new Token(266,currentLineNum);
						}else if(str.compare("print")==0){
							token = new Token(269,currentLineNum);
						}else{
							token = new Token(270,currentLineNum,str);
						}
						break;
					case 7:
						if(str.compare("println")==0){
							token = new Token(273,currentLineNum);
						}else{
							token = new Token(270,currentLineNum,str);
						}
						break;
					case 8:
						if(str.compare("continue")==0){
							token = new Token(267,currentLineNum);
						}else{
							token = new Token(270,currentLineNum,str);
						}
						break;
					default:
						token = new Token(270,currentLineNum,str);
						break;                   
				}
				str = "";
				break;
			case EOF:
				//EOF
				//cout << "End of file." << endl;
				token = NULL;
				break;
			default:
				//ERROR
				cerr << "Line " << currentLineNum << " <Scanner> Unknow ERROR" << endl;
				token = NULL;
				break;
		}
		return token;
	}else{
		return NULL;
	}
}

bool Scanner::scanEof(){
	if(fin.eof()){
		return true;
	}else{
		return false;
	}
}

bool Scanner::scan(){

	Token* token;

	if(fin.fail())
		return false;

	while(!scanEof()){
		token = scanNextToken();
		if(token!=NULL){
			tokenString.push_back(token);
		}
	}

	if(error)
		return false;

	return true;
}

Token* Scanner::getNextToken(){
	if( currentTokenIndex < tokenString.size() ){
		return tokenString[currentTokenIndex++];
	}else{
		return NULL;
	}
}

Token* Scanner::peekNextToken(){
	if( currentTokenIndex < tokenString.size() ){
		return tokenString[currentTokenIndex];
	}else{
		return NULL;
	}
}

	Token* Scanner::getPreviousToken(){
		if( currentTokenIndex > 0)
			return tokenString[currentTokenIndex-1];
	}

bool Scanner::eof(){
	if( currentTokenIndex >= tokenString.size() ){
		return true;
	}else{
		return false;
	}
}

