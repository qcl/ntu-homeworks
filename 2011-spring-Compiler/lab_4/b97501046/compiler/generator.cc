/*
	NTU CSIE Compiler 2011 Lab 4
	B97501046 Qing-Cheng Li
	
	generator.cc
	Java bytecode assambly code generator for Lab4 
*/
#include "generator.h"

Generator::Generator(char *outputFileName,char * _programName){
	fout.open(outputFileName);
	programName = _programName;
	varCounter = 0;
	labelCounter = 0;
}

Generator::~Generator(){
	fout.close();
}

void Generator::header(){
	
	fout << ".class " << programName << endl << endl;
	fout << ".method void <init>()" << endl;
	fout << "\taload #0" << endl;
	fout << "\tinvokespecial void <init>() @ Object" << endl;
	fout << "\treturn" << endl << endl;
	fout << ".method public static void main(String[])" << endl;
}

void Generator::eop(){
	fout << "\treturn" << endl;
}

void Generator::newScope(){
	//cout << "New Scope" << endl;
	//cout << "1.Stack Size = " << tableStack.size() << endl;
	//cout << "crtTlb size = " << currentTable.size() << endl;
	tableStack.push(currentTable);
	//cout << "2.Stack Size = " << tableStack.size() << endl;
}

void Generator::endScope(){
	//cout << "End Scope" << endl;
	//cout << "1.Stack Size = " << tableStack.size() << endl;
	//cout << "crtTlb size = " << currentTable.size() << endl;
	//map<string,pair<int,int> >::iterator map_it;
	//for(map_it=currentTable.begin();map_it!=currentTable.end();map_it++){
	//	cout << map_it->first << ":" << map_it->second.first << "-" << map_it->second.second << endl;
	//}
	currentTable = tableStack.top();
	tableStack.pop();
	//cout << "2.Stack Size = " << tableStack.size() << endl;
	//cout << "--" << endl;
}

void Generator::addId(string id,int d){
	//cout << "addId " << id << "(" << d << ")" << endl;
	currentTable[id] = pair<int,int>(varCounter++,d);
}

int Generator::getIndex(string var){
	map<string,pair<int,int> >::iterator it;
	it = currentTable.find(var);
	if(it!=currentTable.end())
		return it->second.first;
	else
		return -1;
}

int Generator::getDim(string var){
	map<string,pair<int,int> >::iterator it;
	it = currentTable.find(var);
	if(it!=currentTable.end())
		return it->second.second;
	else
		return -1;	
}

int Generator::getNewLabel(){
	return labelCounter++;
}

void Generator::putLabel(int label){
	fout << "L" << label << ":" << endl;
}

void Generator::jump(int label){
	fout << "\tgoto L" << label << endl;
}

void Generator::ifne(int label){
	fout << "\tifne L" << label << endl;
}

void Generator::ifeq(int label){
	fout << "\tifeq L" << label << endl;
}

void Generator::if_icmpne(int label){
	fout << "\tif_icmpne L" << label << endl;
}

void Generator::if_icmpeq(int label){
	fout << "\tif_icmpeq L" << label << endl;
}

void Generator::if_icmpge(int label){
	fout << "\tif_icmpge L" << label << endl;
}

void Generator::if_icmpgt(int label){
	fout << "\tif_icmpgt L" << label << endl;
}

void Generator::if_icmple(int label){
	fout << "\tif_icmple L" << label << endl;
}

void Generator::if_icmplt(int label){
	fout << "\tif_icmplt L" << label << endl;
}

void Generator::newArray(int index,vector<string> d){
	for(int i=0;i<d.size();i++){
		fout << "\tldc " << d[i] << endl;
	}
	
	if(d.size()==1){
		fout << "\tnewarray int" << endl;
	}else{
		fout << "\tmultianewarray int";
		for(int i=0;i<d.size();i++){
			fout << "[]";
		}
		fout << " " << d.size() << "-d" << endl;
	}
	fout << "\tastore #" << index << endl;
}

void Generator::ldc(string str){
	fout << "\tldc \"" << str << "\"" << endl;
}

void Generator::ldcInt(string str){
	fout << "\tldc " << str << endl;
}

void Generator::iload(int index){
	fout << "\tiload #" << index << endl;
}

void Generator::istore(int index){
	fout << "\tistore #" << index << endl;
}

void Generator::aload(int index){
	fout << "\taload #" << index << endl;
}

void Generator::aaload(){
	fout << "\taaload" << endl;
}

void Generator::iaload(){
	fout << "\tiaload" << endl;
}

void Generator::iastore(){
	fout << "\tiastore" << endl;
}

void Generator::iadd(){
	fout << "\tiadd" <<endl;
}

void Generator::isub(){
	fout << "\tisub" <<endl;
}

void Generator::imul(){
	fout << "\timul" <<endl;
}

void Generator::idiv(){
	fout << "\tidiv" <<endl;
}

void Generator::ineg(){
	fout << "\tineg" <<endl;
}

void Generator::printString(){
	fout << "\tinvokestatic void print(String) @ StdIO" << endl;
}

void Generator::printInt(){
	fout << "\tinvokestatic void print(int) @ StdIO" << endl;
}

void Generator::println(){
	fout << "\tinvokestatic void println() @ StdIO" << endl;
}

void Generator::scan(){
	fout << "\tinvokestatic int readInt() @ StdIO" << endl;
}

