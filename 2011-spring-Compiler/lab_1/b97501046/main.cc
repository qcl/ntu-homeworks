/*
    NTU CSIE Compiler 2011 Lab 1
    B97501046 Qing-Cheng Li
    
    main.cc
    The main program of lab 1, it call scanner and output the result.
*/
#include <iostream>
#include <fstream>
#include "token.h"
#include "scanner.h"

using namespace std;

int main(int argc,char * argv[]){
    
    char * inputFileName;
    char * outputFileName;
    ofstream fout;
    
    if(argc!=3){
        cout << "usage: main [input file name] [output file name]" << endl;
        return 0;
    }
    
    inputFileName = argv[1];
    outputFileName = argv[2];
    
    Scanner* scanner = new Scanner(inputFileName);
    Token* token;
    int tokenCode;
    string attr;
    
    fout.open(outputFileName);
    
    while(!scanner->eof()){
        token = scanner->getNextToken();
        if(token!=NULL){
            tokenCode = token->getTokenCode();
            fout << tokenCode;
            if(token->hasAttribute()){
                attr = token->getAttribute();
                fout << "\t" << attr;
            }
            fout << endl;
        }
    }
    
    return 0;
}

