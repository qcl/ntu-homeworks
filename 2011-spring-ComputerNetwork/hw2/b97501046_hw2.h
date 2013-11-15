/*
	Computer Network 2011 Spring Programming Assignment 2
	B97501046 Qing-Cheng Li
	
	b97501046_hw2.h
	
*/
#ifndef _B97501046_HW2_H_
#define _B97501046_HW2_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

enum{
	PKT_MAX_SIZE = 1071,
	PKT_ACK_SIZE = 43,
	PKT_DATA_MIN_SIZE = 47,
	PKT_DATA_MAX_SIZE = 1024,
	PKT_TYPE = 36,
	PKT_TYPE_FILENAME = 1,
	PKT_TYPE_DATA = 2,
	PKT_TYPE_ACK = 3,
	PKT_TYPE_FIN = 4,
	PKT_SEND_IP = 0,
	PKT_RECV_IP = 18,
	PKT_SEND_PORT = 16,
	PKT_RECV_PORT = 34,
	PKT_AGENT_PORT = 37,
};

struct transSetting{	
	int send;
	int send_port;
	
	int target_port;
	char target_host[16];
	
	int recv;
	int recv_port;
	
	int agent;
	int agent_port;
	char agent_host[16];
	
	int rename;
	char *inputFilename;
	char *outputFilename;
};

struct transSetting* init_trans(int argc, char *argv[]);

int makePktHeader(char *,char *,int,char*,int,int,int,int,int);

int getPktSendPort(unsigned char*);

int getPktRecvPort(unsigned char*);

int getPktSeqNum(unsigned char*);

int getPktDataLeng(unsigned char*);

int getPktAgentPort(unsigned char*);

#endif
