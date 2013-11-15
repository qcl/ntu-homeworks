/*
	Computer Network 2011 Homework 1
	b97501046 Qing-Cheng Li

	SimpleMail
	b97501046.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SMCF.h"

int main(int argc,char *argv[]){
	
	int n;
	int sockfd;
	int c_to;	//counter of to
	int c_cc;	//counter of cc
	int c_bcc;	//counter of bcc
	int c_file;	//counter of file
	char addr[16];	//ip addr
	char from[1024];	//sender
	char mailbody[1024];	//mail body
	char subject[1024];	//subject
	char* to[1024];	//to
	char* cc[1024];	//cc
	char* bcc[1024];	//bcc
	char* file[1024];	//files

	/* Check arguments*/
	if(argc!=3){
		printf("usage: ./SimpleMail [host] [port]\n");
		return 0;
	}

	/* Write a mail */
	do{
		welcomeMsg();	
	
		getSender(from);	// Get Sender's email
		c_to = getReciver(to);	// Get Reciver's email
		c_cc = getCc(cc);	// Get Carbon-copy's mail
		c_bcc = getBcc(bcc);	// Get Blind Carbon-copy's mail
		getSubject(subject);	// Get Subject
		getMailbody(mailbody);	// Get Mail Body File
		c_file = getAttachment(file);	// Get Attachment Files

		/* Checking mail */
		n = checkMail(from,to,c_to,cc,c_cc,bcc,c_bcc,subject,mailbody,file,c_file);
		if(n==-1){
			printf("Bye\n");
			return 0;
		}
	}while(n==0);	
	
	/* Connect to mail server */
	sockfd = connectToHost(argv[1],argv[2],addr);
	if(sockfd==-1){
		/* Connect Fail */
		printf("Connect to host %s:%s fail\n",argv[1],argv[2]);
		return 0;
	}else{
		printf("Connect to host %s(%s):%s\n",argv[1],addr,argv[2]);
		
		n = HELO(sockfd);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			return 0;
		}
		
		n = MAIL(sockfd,from);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			return 0;
		}
		
		n = RCPT(sockfd,to,c_to,cc,c_cc,bcc,c_bcc);
		if(n==-1||n==-2||n==-3){
			close(sockfd);
			printf("Close SimpleMail.\n");
			return 0;
		}
		
		n = MAIL_Headr(sockfd,from,to,c_to,cc,c_cc,bcc,c_bcc,subject,c_file);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			return 0;
		}
		
		MAIL_body(sockfd,mailbody,c_file);
		Attachment(sockfd,file,c_file);
		
		n = EOM(sockfd,c_file);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			return 0;
		}
		
		n = QUIT(sockfd);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			return 0;
		}
		
		printf("Send mail.\n");
		close(sockfd);	
	}
	
	return 0;
}

