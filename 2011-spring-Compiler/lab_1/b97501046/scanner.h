/*
    NTU CSIE Compiler 2011 Lab 1
    B97501046 Qing-Cheng Li
    
    scanner.h
    The header file of object scanner
*/
#ifndef _SCANNER_H_
#define _SCANNER_H_
#include <fstream>
#include "token.h"

using namespace std;

class Scanner{
    private:
        ifstream fin;
    public:
        Scanner(char*);
        ~Scanner();
        Token* getNextToken();
        bool eof();
};

#endif
