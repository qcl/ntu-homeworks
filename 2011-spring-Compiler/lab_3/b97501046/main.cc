/*
    NTU CSIE Compiler 2011 Lab 3
    B97501046 Qing-Cheng Li
    
    main.cc
    The main program of lab 3.
*/
#include <iostream>
#include <fstream>
#include "parser.h"

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
    
    /* Parsing */
    Parser* parser = new Parser(inputFileName);
    parser->parse();
    
    /* Output result */
    fout.open(outputFileName);
    
    if(parser->isPass())
        fout << "Pass" << endl;
    else
        fout << "Fail" << endl;
    
    fout.close();
    
    return 0;
}

