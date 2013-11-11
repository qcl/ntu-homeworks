/*
    NTU CSIE Compiler 2011 Lab 3
    B97501046 Qing-Cheng Li
    
    scanner.h
    The header file of object scanner
*/
#ifndef _SCANNER_H_
#define _SCANNER_H_
#include <fstream>
#include <vector>
#include "token.h"

using namespace std;

class Scanner{
    private:
        ifstream fin;
        int currentLineNum;
        int currentTokenIndex;
        bool error;
        vector<Token*> tokenString;
        Token* scanNextToken();
        bool scanEof();
    public:
        Scanner(char*);
        ~Scanner();
        bool scan();
        Token* getNextToken();
        Token* peekNextToken();
        Token* getPreviousToken();
        bool eof();
};

#endif
