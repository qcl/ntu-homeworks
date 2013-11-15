/*
	Computer Network 2011 Homework 1
	b97501046 Qing-Cheng Li

	SimpleMail
	SMCF.h - Simple Mail Client Function
*/

#ifndef _SMCF_H_
#define _SMCF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void welcomeMsg();
void getSender(char *sender);
int getReciver(char **reciver);
int getCc(char **cc);
int getBcc(char **bcc);
void getSubject(char *subject);
void getMailbody(char *mailbody);
int getAttachment(char **file);

int checkMail(
	char *from,
	char **to,int c_to,
	char **cc,int c_cc,
	char **bcc,int c_bcc,
	char *subject,
	char *mailbody,
	char **file,int c_file);

int connectToHost(char *host, char *port, char* addr);

int HELO(int fd);
int MAIL(int fd,char *from);
int RCPT(int fd,char **to,int c_to,char **cc, int c_cc, char **bcc, int c_bcc);
int MAIL_Headr(int fd,char *from,char **to, int c_to,char **cc,int c_cc,char **bcc,int c_bcc,char *subject,int c_file);
int MAIL_body(int fd,char *mailbody,int c_file);
int Attachment(int fd,char **file,int c_file);
int EOM(int fd,int c_file);
int QUIT(int fd);

char base64encode(unsigned char a);

#endif
