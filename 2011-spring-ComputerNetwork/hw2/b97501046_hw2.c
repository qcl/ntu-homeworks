/*
	Computer Network 2011 Spring Programming Assignment 2
	B97501046 Qing-Cheng Li
	
	b97501046_hw2.c
	
*/
#include "b97501046_hw2.h"

struct transSetting* init_trans(int argc, char *argv[]){
	
	if(argc<=1){
		printf("using ``-h'' to find more infomation.\n");
		return NULL;
	}else if(strcmp(argv[1],"-h")==0){
		printf("\nCN 2011 Spring Programming Assignment 2\n");
		printf("B97501046 Qing-Cheng Li\n\n");
		printf("Usage:\n");
		printf("Be a Sender:\n");
		printf("\t./trans -s -p recieve_port -t target_host -tp target_port\n\t\t-f filename [-o new_filename] [-a agent_host -ap agent_port]\n");
		printf("Be a Receiver:\n");
		printf("\t./trans -r -p receive_port [-o filename]\n\n");
		return NULL;
	}else{
		
		struct transSetting* setting = malloc(sizeof(struct transSetting));
		struct hostent *queryHost;
		int i;
		
		/* Init setting*/
		setting->send = 0;
		setting->recv = 0;
		setting->agent = 0;
		setting->rename = 0;
				
		for(i=0;i<argc;i++){
			if(strcmp(argv[i],"-s")==0){
				/* Be a Sender */
				setting->send = 1;
			}else if(strcmp(argv[i],"-t")==0){
				/* Get target host */
				queryHost = gethostbyname(argv[++i]);
				inet_ntop(AF_INET,queryHost->h_addr,setting->target_host,sizeof(char)*16);
			}else if(strcmp(argv[i],"-tp")==0){
				/* Get target port */
				setting->target_port = atoi(argv[++i]);
			}else if(strcmp(argv[i],"-p")==0){
				if(setting->send==1)
					/* Get sender's port */
					setting->send_port = atoi(argv[++i]);
				else
					/* Get receiver's port */
					setting->recv_port = atoi(argv[++i]);
			}else if(strcmp(argv[i],"-f")==0){
				/* Get input filename */
				setting->inputFilename = argv[++i];
			}else if(strcmp(argv[i],"-a")==0){
				/* Get agent host */
				setting->agent = 1;
				queryHost = gethostbyname(argv[++i]);
				inet_ntop(AF_INET,queryHost->h_addr,setting->agent_host,sizeof(char)*16);
			}else if(strcmp(argv[i],"-ap")==0){
				/* Get agent port */
				setting->agent_port = atoi(argv[++i]);
			}else if(strcmp(argv[i],"-r")==0){
				/* Be a receiver */
				setting->recv = 1;
			}else if(strcmp(argv[i],"-o")==0){
				/* Get output filename */
				setting->rename = 1;
				setting->outputFilename = argv[++i];
			}
		}
		return setting;
	}
	return NULL;
}

/*
	makePktHeader
	Make Packet Header
	Type:
		1	filename (data)
		2	data
		3	ack
		4	fin
*/
int makePktHeader(char *pkt,char *send_host,int send_port,char *target_host,int target_port,int type,int agent_port,int seq,int length){
	
	int j;
	int tmp;
	
				
	/* send_host */
	j=0;
	while(1){
		if(send_host[j]!='\0'){
			pkt[j] = send_host[j]; 
			j++;
		}else{
			pkt[j] = '\0';
			break;
		}
	}
								
	/* sned_port */
	tmp = send_port;
	pkt[17] = tmp%256;
	tmp /= 256;
	pkt[16] = tmp%256;
		
	/* target_host */
	j=0;
	while(1){
		if(target_host[j]!='\0'){
			pkt[18+j] = target_host[j]; 
			j++;
		}else{
			pkt[18+j] = '\0';
			break;
		}
	}
			
	/* target_port */
	tmp = target_port;
	pkt[35] = tmp%256;
	tmp /= 256;
	pkt[34] = tmp%256;
	
	/* type */
	pkt[36] = type;

	/* agent_port */
	tmp = agent_port;
	pkt[38] = tmp%256;
	tmp/=256;
	pkt[37] = tmp%256;

	/* seq */
	tmp = seq;
	pkt[42] = tmp%256;
	tmp /= 256;
	pkt[41] = tmp%256;
	tmp /= 256;
	pkt[40] = tmp%256;
	tmp /= 256;
	pkt[39] = tmp%256;

	if(type>2)
		return 43;
	/* length */
	tmp = length;
	pkt[46] = tmp%256;
	tmp /= 256;
	pkt[45] = tmp%256;
	tmp /= 256;
	pkt[47] = tmp%256;
	tmp /= 256;
	pkt[43] = tmp%256;

	return 47 + length;
}

/* Get sender's port from packet*/
int getPktSendPort(unsigned char* pkt){
	return pkt[16]*256+pkt[17];
}

/* Get receiver's port from packet*/
int getPktRecvPort(unsigned char* pkt){
	return pkt[34]*256+pkt[35];
}

/* Get seq# from packet*/
int getPktSeqNum(unsigned char* pkt){
	return ((((pkt[39]*256)+pkt[40])*256)+pkt[41])*256+pkt[42];
}

/* Get data length from packet*/
int getPktDataLen(unsigned char* pkt){
	return ((((pkt[43]*256)+pkt[44])*256)+pkt[45])*256+pkt[46];
}

/* Get agent's port from packet*/
int getPktAgentPort(unsigned char* pkt){
	return pkt[37]*256+pkt[38];
}


