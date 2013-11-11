/*
    NTU CSIE Compiler 2011 Lab 2
    B97501046 Qing-Cheng Li
    
    scanner.cc
    Modified from scanner.cc of lab 1.
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

/*
    Token code for Lab 2
    
    :       1
    |       2
    ;       3
    string  4
    id      5
*/

Token* Scanner::getNextToken(){
    if(!fin.eof()){
        Token* token;
        string str;     //used to record attribute
        int n;
        char c = fin.get();
        switch(c){
            case ':':
                token = new Token(1);
                break;
            case '/':
                c = fin.get();
                if(c!='*'){
                    //ERROR
                    cerr << "Undefine char `/'" << endl;
                    token = NULL;
                    fin.unget();
                }else{  //Deal with comment
                    //Start of comment
                    while(true){
                        c = fin.get();
                        if(c=='*'){
                            c = fin.get();
                            if(c=='/'){ //End of comment
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
            case '|':
                c = fin.get();
                token = new Token(2);
                break;
            case ';':
                token = new Token(3);
                break;
            case 0x20:
            case 0x09:
            case 0x0d:
            case 0x0a:
                token = getNextToken();
                break;
            case '"':   /* String */
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
                token = new Token(4,str);
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
                /* Identifier */
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
                token = new Token(5,str);
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

