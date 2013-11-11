/*
    NTU CSIE Compiler 2011 Lab 1
    B97501046 Qing-Cheng Li
    
    scanner.cc
    The scanner of cmm
*/
#include "scanner.h"
#include <string>
#include <iostream>

using namespace std;

Scanner::Scanner(char * inputFileName){
    fin.open(inputFileName);
}

Scanner::~Scanner(){

}

Token* Scanner::getNextToken(){
    if(!fin.eof()){
        Token* token;
        string str;     //used to record attribute
        char num[11];   //used to record number
        int n;
        char c = fin.get();
        switch(c){
            case '+':
                token = new Token(43);
                break;
            case '-':
                token = new Token(45);
                break;
            case '*':
                token = new Token(42);
                break;
            case '/':
                c = fin.get();
                if(c!='*'){
                    token = new Token(47);
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
                                token = getNextToken(); //Get Next Token then return it.
                                break;
                            }else{
                                fin.unget();
                            }
                        }else if(c==EOF){
                            //ERROR
                            //return a null token and output error msg
                            cerr << "Comment cannot find */" << endl;
                            token = NULL;
                            break;
                        }
                    }
                }
                break;
            case '&':
                c = fin.get();
                if(c=='&'){
                    token = new Token(256);
                }else{
                    //ERROR
                    //return a null token and output error msg
                    cerr << "& must follow by another & like &&" << endl;
                    token = NULL;
                    fin.unget();
                }
                break;
            case '|':
                c = fin.get();
                if(c=='|'){
                    token = new Token(257);
                }else{
                    //ERROR
                    //return a null token and output error msg
                    cerr << "| must follow by another | like ||" << endl;
                    token = NULL;
                    fin.unget();
                }
                break;
            case '!':
                c = fin.get();
                if(c=='='){
                    token = new Token(261);
                }else{
                    token = new Token(33);
                    fin.unget();
                }
                break;
            case '<':
                c = fin.get();
                if(c=='='){
                    token = new Token(258);
                }else{
                    token = new Token(60);
                    fin.unget();
                }
                break;
            case '>':
                c = fin.get();
                if(c=='='){
                    token = new Token(259);
                }else{
                    token = new Token(62);
                    fin.unget();
                }
                break;
            case '=':
                c = fin.get();
                if(c=='='){
                    token = new Token(260);
                }else{
                    token = new Token(61);
                    fin.unget();
                }
                break;
            case '(':
                token = new Token(40);
                break;
            case ')':
                token = new Token(41);
                break;
            case '[':
                token = new Token(91);
                break;
            case ']':
                token = new Token(93);
                break;
            case '{':
                token = new Token(123);
                break;
            case '}':
                token = new Token(125);
                break;
            case ',':
                token = new Token(44);
                break;
            case ';':
                token = new Token(59);
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
                    cerr << "Number must less than 999999" << endl;
                }
                sprintf(num,"0x%08x\0",n);
                str = string(num);
                token = new Token(271,str);
                str = "";   //clear str
                break;
            case 0x20:
            case 0x09:
            case 0x0d:
            case 0x0a:
                token = getNextToken();
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
                        cerr << "String cannot find anothrt \"" << endl;
                        break;
                    }else{
                        //ERROR
                        //recovery: don't add those unprintabl character to str
                        cerr << "String contant unprintable characters" << endl;
                    }
                }
                token = new Token(272,str);
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
                            token = new Token(263);
                        }else{
                            token = new Token(270,str);
                        }
                        break;
                    case 3:
                        if(str.compare("int")==0){
                            token = new Token(262);
                        }else{
                            token = new Token(270,str);
                        }
                        break;
                    case 4:
                        if(str.compare("else")==0){
                            token = new Token(264);
                        }else if(str.compare("scan")==0){
                            token = new Token(268);
                        }else{
                            token = new Token(270,str);
                        }
                        break;
                    case 5:
                        if(str.compare("while")==0){
                            token = new Token(265);
                        }else if(str.compare("break")==0){
                            token = new Token(266);
                        }else if(str.compare("print")==0){
                            token = new Token(269);
                        }else{
                            token = new Token(270,str);
                        }
                        break;
                    case 8:
                        if(str.compare("continue")==0){
                            token = new Token(267);
                        }else{
                            token = new Token(270,str);
                        }
                        break;
                    default:
                        token = new Token(270,str);
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
                cerr << "Unknow ERROR" << endl;
                token = NULL;
                break;
        }
        return token;
    }else{
        return NULL;
    }
}

bool Scanner::eof(){
    if(fin.eof()){
        return true;
    }else{
        return false;
    }
}

