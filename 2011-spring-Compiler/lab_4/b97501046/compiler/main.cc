/*
	NTU CSIE Compiler 2011 Lab 4
	B97501046 Qing-Cheng Li
	
	main.cc
	The main program of lab 4.
*/
#include <iostream>
#include "parser.h"

using namespace std;

int main(int argc,char * argv[]){
	
	char * inputFileName;
	char * outputFileName;
	char * programName;
	
	if(argc!=4){
		cout << "usage: main [input file] [output file] [program name]" << endl;
		return 0;
	}
	
	inputFileName = argv[1];
	outputFileName = argv[2];
	programName = argv[3];
	
	/* Parsing */
	Parser* parser = new Parser(inputFileName,outputFileName,programName);
	parser->parse();
	
	if(parser->isPass())
		cout << "Pass" << endl;
	else
		cout << "Fail" << endl;
	
	return 0;
}

