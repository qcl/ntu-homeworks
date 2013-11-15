/*
	Computer Network 2011 Homework 3
	Ping Talk

	b97501046 Qing-Cheng Li
	b97501046_hw3_pingtalk.cpp - main program of this homework.
*/
#include <iostream>
#include <string>
#include "cnping.h"

using namespace std;

int main(int argc,char* argv[]){

	Ping* ping = new Ping();

	if(ping->init(argc,argv)){
		if(ping->setting()){
			ping->main_loop();
		}
	}

	return 0;
}

