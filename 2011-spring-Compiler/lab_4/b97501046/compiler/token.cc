/*
	NTU CSIE Compiler 2011 Lab 4
	B97501046 Qing-Cheng Li
	
	token.cc
	Token object which the scanner return to parser.
*/
#include "token.h"

Token::Token(int code, int line){
	tokenCode = code;
	position = line;
	attribute = "";
	hasAttr = false;
}

Token::Token(int code, int line, string attr){
	tokenCode = code;
	position = line;
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

int Token::getPosition(){
	return position;
}

