/*
    NTU CSIE Compiler 2011 Lab 3
    B97501046 Qing-Cheng Li
    
    token.h
    The header file of token.cc.
*/
#ifndef _TOKEN_H_
#define _TOKEN_H_
#include <string>
using namespace std;

class Token{
    private:
        int tokenCode;
        int position;
        string attribute;
		bool hasAttr;
    public:
        Token(int,int);
        Token(int,int,string);
        ~Token();
        int getTokenCode();
        bool hasAttribute();
        string getAttribute();
        int getPosition();
};

#endif
