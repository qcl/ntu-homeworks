/*
	Computer Network 2011 Homework 1
	b97501046 Qing-Cheng Li

	SimpleMail
	SMCF_GUI.h - Simple Mail Client GUI Function
*/
#ifndef _SMCF_GUI_H_
#define _SMCF_GUI_H_
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SMCF.h"

struct GUI{
	GtkWidget *window;
	
	GtkWidget *iptPort;
	GtkWidget *iptHost;
	
	GtkWidget *iptFrom;
	GtkWidget *iptTo[1024];
	GtkWidget *btnAddTo;
	GtkWidget *iptCc[1024];
	GtkWidget *btnAddCc;
	GtkWidget *iptBcc[1024];
	GtkWidget *btnAddBcc;
	GtkWidget *iptSubject;
	GtkWidget *btnOpenmail;
	GtkWidget *btnOpenfile[1024];
	GtkWidget *btnAddFile;
	
	GtkWidget *btnSend;
	
	GtkWidget *table;
	GtkWidget *tableTo;
	GtkWidget *tableCc;
	GtkWidget *tableBcc;
	GtkWidget *tableFile;
	GtkWidget *tableCon;

	GtkWidget *statu[8];
	
	int c_to;	//counter of to
	int c_cc;	//counter of cc
	int c_bcc;	//counter of bcc
	int c_file;	//counter of file
	char *from;	//sender
	char *mailbody;	//mail body
	char *subject;	//subject
	char* to[1024];	//to
	char* cc[1024];	//cc
	char* bcc[1024];	//bcc
	char* file[1024];	//files
	
	char *host;
	char *port;
};


void addTo(GtkWidget* widget,struct GUI *gui);
void addCc(GtkWidget* widget,struct GUI *gui);
void addBcc(GtkWidget* widget,struct GUI *gui);
void addFile(GtkWidget* widget,struct GUI *gui);
void sendMail(GtkWidget* widget,struct GUI *gui);
void initGui(struct GUI *gui);

#endif

