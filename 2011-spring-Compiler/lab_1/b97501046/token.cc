/*
    NTU CSIE Compiler 2011 Lab 1
    B97501046 Qing-Cheng Li
    
    token.cc
    Token object which the scanner return to parser.
*/
#include "token.h"

Token::Token(int code){
    tokenCode = code;
    attribute = "";
	hasAttr = false;
}

Token::Token(int code, string attr){
    tokenCode = code;
    attribute = attr;
	hasAttr = true;
}

Token::~Token(){

}

int Token::getTokenCode(){
    return tokenCode;
}

bool Token::hasAttribute(){
	return hasAttr;
}

string Token::getAttribute(){
    return attribute;
}

