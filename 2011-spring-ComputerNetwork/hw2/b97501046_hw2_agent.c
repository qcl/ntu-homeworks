/*
	Computer Network 2011 Spring Programming Assignment 2
	B97501046 Qing-Cheng Li
	
	b97501046_hw2_agent.c
*/
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include "b97501046_hw2.h"

int main(int argc, char* argv[]){
	
	if(argc<=1){
		printf("using ``-h'' to find more infomation.\n");
		return 0;
	}else if(strcmp(argv[1],"-h")==0){
		printf("\nCN 2011 Spring Programming Assignment 2\n");
		printf("B97501046 Qing-Cheng Li\n\n");
		printf("Usage:\n");
		printf("Be a Agent:\n");
		printf("\t./agent -p listen_port -l loss_rate\n\n");
	}else{
		
		int i,j;
		int listen_port;
		int listen_fd;
		int max_fd_num = getdtablesize();
		int select_num;
		double lr_set;	//setted loss rate
		double lr_cur;	//current loss rate
		int lpn = 0;	//loss pkt num
		int rpn = 0;	//all data pkt num
		int conn_fd;
		unsigned char pkt[PKT_MAX_SIZE];
		int pktLen;
		int fromSender;
		unsigned char in_ip[16];
		unsigned char in_port[2];
		unsigned char out_ip[16];
		unsigned char out_port[2];
		
		struct sockaddr_in
			serverAddress,
			recvAddress,
			targetAddress;
		struct timeval timeout;
		socklen_t socklen = sizeof(recvAddress);
		fd_set reading_set;
		
		//Init 
		for(i=1;i<argc;i++){
			if(strcmp(argv[i],"-p")==0){
				listen_port = atoi(argv[++i]);
			}else if(strcmp(argv[i],"-l")==0){
				lr_set = atof(argv[++i]);
			}
		}		
		srand ( time(NULL) );

		printf("Agent (listen port:%d)\nloss rate:%.4f\n",listen_port,lr_set);
	
		//Setting Agent Server	
		listen_fd = socket(AF_INET,SOCK_DGRAM,0);
		
		bzero(&serverAddress,sizeof(serverAddress));
		
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port   = htons(listen_port);
		serverAddress.sin_addr.s_addr = 
								   htonl(INADDR_ANY);
		
		bind(listen_fd,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
		
		listen(listen_fd,1);
		
		lpn = rpn = 0;
		
		while(1){	//main loop
			FD_ZERO(&reading_set);
			FD_SET(listen_fd,&reading_set);
			timeout.tv_sec = 10;	//set timeout
			timeout.tv_usec = 0;
			
			select_num = select(max_fd_num,&reading_set,
				NULL,NULL,&timeout);
			
			if(select_num<0){
				//error
			}else if(select_num>0){
			
			if(FD_ISSET(listen_fd,&reading_set)){
				
				//recv pkt
				pktLen = recvfrom(listen_fd,pkt,sizeof(char)*PKT_MAX_SIZE,0,(struct sockaddr*)&recvAddress,&socklen);
				
				int targetPort = getPktRecvPort(pkt);
				int seqNum = getPktSeqNum(pkt);

				//get some infomation from pkt
				for(i=0;i<16;i++){
					in_ip[i] = pkt[PKT_SEND_IP+i];
					out_ip[i] = pkt[PKT_RECV_IP+i];
				}
				for(i=0;i<2;i++){
					in_port[i] = pkt[PKT_SEND_PORT+i];
					out_port[i] = pkt[PKT_RECV_PORT+i];
				}
								
				//preparing fwd
				bzero(&targetAddress,sizeof(targetAddress));
				targetAddress.sin_family = AF_INET;
				targetAddress.sin_port   = htons(targetPort);
				inet_pton(AF_INET,out_ip,
					&targetAddress.sin_addr);
				
				//if pkt from sender, modify it.
				fromSender = 0;
				if(strcmp(&pkt[PKT_SEND_IP],"0.0.0.0")==0){
					//from sender
					fromSender =1;
					char ip[16];
					inet_ntop(AF_INET,&recvAddress.sin_addr,ip,sizeof(char)*16);
					//printf("add %s \n",ip);
					for(i=0;i<16;i++){
						pkt[PKT_SEND_IP+i] = ip[i];
					}
				}
				
				if(pkt[PKT_TYPE]<=2){
					//filename, data
					printf("get  data #%d\n",seqNum);
					
					if( rand()%1000+1 < lr_set*1000 ){
						
						//loss pkt
						
						lpn++;
						rpn++;
						printf("drop data #%d, loss rate = %.4f\n",seqNum,((double)lpn/(double)rpn));
					
					}else{
						
						//forwarding pkt
						
						rpn++;
						printf("fwd  data #%d, loss rate = %.4f\n",seqNum,(double)lpn/(double)rpn);
						
						conn_fd = socket(AF_INET,SOCK_DGRAM,0);
						j = connect(conn_fd,
							(struct sockaddr*)&targetAddress,
							sizeof(targetAddress));
						if(j==0){
							j = write(conn_fd,pkt,pktLen);
							//printf("fwd len = %d\n",j);
							close(conn_fd);					
						}
					}
					
				}else{
					//ack, fin
					
					if(pkt[PKT_TYPE]==PKT_TYPE_FIN){
						if(fromSender==1){
							printf("get  fin\n");
							printf("fwd  fin\n");
						}else{
							printf("get  ack  fin\n");
							printf("fwd  ack  fin\n");
						}
					}else{
						printf("get  ack  #%d\n",seqNum);
						printf("fwd  ack  #%d\n",seqNum);
					}
					
					// forwarding ack/fin
					conn_fd = socket(AF_INET,SOCK_DGRAM,0);
					j = connect(conn_fd,(struct sockaddr*)&targetAddress,sizeof(targetAddress));
					
					if(j==0){
						j = write(conn_fd,pkt,pktLen);
						//printf("fwd len = %d\n",j);
						close(conn_fd);					
					}
					
					if(pkt[PKT_TYPE]==PKT_TYPE_FIN&&fromSender==0){
						printf("Done.\n");
						break;
					}
				}
				
			}
				
			}else{
				//timeout
			}
			
		}//end of while
		
	}

	return 0;
	
}

