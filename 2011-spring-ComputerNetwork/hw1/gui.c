/*
	Computer Network 2011 Homework 1
	b97501046 Qing-Cheng Li

	SimpleMail
	gui.c
*/
#include <stdio.h>
#include <stdlib.h>
#include "SMCF_GUI.h"

int main(int argc,char *argv[]){
		
	struct GUI* gui = malloc(sizeof(struct GUI));
	
	welcomeMsg();
	
	gtk_init(&argc,&argv);
	initGui(gui);
	gtk_widget_show_all(gui->window);
	gtk_main();

	return 0;
}
