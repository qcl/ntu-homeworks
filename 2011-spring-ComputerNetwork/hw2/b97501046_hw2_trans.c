/*
	Computer Network 2011 Spring Programming Assignment 2
	B97501046 Qing-Cheng Li
	
	b97501046_hw2_trans.c	
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
#include "b97501046_hw2.h"

int main(int argc, char* argv[]){
	
	/* Init setting */
	struct transSetting* setting = init_trans(argc,argv);
	if(setting==NULL){
		return 0;
	}
	
	if(setting->send==1){
		/* Be a Sender */

		int i,j,c;
		int conn_fd;
		struct sockaddr_in serverAddress,recvAckAddress,clientAddress;
		struct timeval timeout;
		int listen_fd;
		int input_fd;
		int select_num;
		int max_fd_num = getdtablesize();
		fd_set reading_set;
		socklen_t socklen = sizeof(clientAddress);;
		unsigned char packet[PKT_MAX_SIZE];
		unsigned char ackpkt[PKT_ACK_SIZE];
		char outName[1024];
		int len;
		int lastSeq;
		int maxHasSend;
		
		/* local send buffer */	
		int hasSend[40];
		int hasAck[40];
		unsigned char Buffer[40][PKT_MAX_SIZE];
		int Buflen[40];

		/* Some init */
		int winSize = 1;
		int threshold = 16; 
		int base = 0;
		int seq=0;
		
		lastSeq = -1;
		maxHasSend = -1;
		
		for(i=0;i<40;i++){
			hasSend[i] = 0;
			hasAck[i]  = 0;
			Buflen[i]  = -1;
		}
		
		/* Setting Ack Recv (As a server )*/
		listen_fd = socket(AF_INET,SOCK_DGRAM,0);	
		bzero(&recvAckAddress,sizeof(recvAckAddress));	
		recvAckAddress.sin_family = AF_INET;
		recvAckAddress.sin_port   = htons(setting->send_port);
		recvAckAddress.sin_addr.s_addr = htonl(INADDR_ANY);	
		bind(listen_fd, (struct sockaddr *)&recvAckAddress,sizeof(recvAckAddress));	
		listen(listen_fd,1);
		
		/* Setting target */
		bzero(&serverAddress, sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		if(setting->agent==1){
			serverAddress.sin_port   = htons(setting->agent_port);
			inet_pton(AF_INET,setting->agent_host,&serverAddress.sin_addr.s_addr);
		}else{
			setting->agent_port=0;
			serverAddress.sin_port   = htons(setting->target_port);
			inet_pton(AF_INET,setting->target_host,&serverAddress.sin_addr.s_addr);
		}
		
		/* Sender main loop */
		while(1){
		
			/* Init for select */
			FD_ZERO(&reading_set);
			FD_SET(listen_fd,&reading_set);
			timeout.tv_sec = 5;
			timeout.tv_usec = 0;
		
			/* send data */
			if( seq < base+winSize ){
				//if(lastSeq>0){
				//	printf("seq=%d base=%d\n",seq,base);
				//}				
				/* Send filename */
				if(seq==0){
					conn_fd = socket(AF_INET,SOCK_DGRAM,0);
					c = connect(conn_fd,(struct sockaddr *) &serverAddress,sizeof(serverAddress));
					if(c==0){
						if(setting->rename==1){
							strcpy(outName,setting->outputFilename);
						}else{
							strcpy(outName,setting->inputFilename);
						}
				
						//printf("|%s|=%d\n",outName,(int)strlen(outName));
						len = makePktHeader(packet,"0.0.0.0",setting->send_port,setting->target_host,setting->target_port,PKT_TYPE_FILENAME,setting->agent_port,seq,(int)strlen(outName));
						strncpy(&packet[PKT_DATA_MIN_SIZE],outName,(int)strlen(outName));
						//local buffer setting 
						hasSend[seq-base] = 1;
						hasAck[seq-base] = 0;
						strncpy(Buffer[seq-base],packet,len);
						Buflen[seq-base] = len;
						
						printf("send  data #%d, winSize = %d\n",seq,winSize);
						int k = write(conn_fd,packet,sizeof(char)*(len));
						fflush(stdout);
						
						input_fd = open(setting->inputFilename,O_RDONLY);
						seq++;
					}
					close(conn_fd);
				}
							
				/* Send data */
				for(i=seq-base;i<winSize;i++){
					
					if(lastSeq<0){
						lseek(input_fd,PKT_DATA_MAX_SIZE*(seq-1),SEEK_SET);
						//printf("i:%d,lsk->%d:",i,1024*(seq-1));
						Buflen[i] = read(input_fd,Buffer[i],PKT_DATA_MAX_SIZE);
						len = makePktHeader(packet,"0.0.0.0",setting->send_port,setting->target_host,setting->target_port,PKT_TYPE_DATA,setting->agent_port,seq,Buflen[i]);
					
						//copy from local buf to pkt
						for(j=0;j<Buflen[i];j++){
							packet[PKT_DATA_MIN_SIZE+j] = Buffer[i][j];
						}
					
						/* Setting connection */
						conn_fd = socket(AF_INET,SOCK_DGRAM,0);
						c = connect(conn_fd,(struct sockaddr *) &serverAddress,	sizeof(serverAddress));
					
						if(c==0){
							if(seq>maxHasSend){
								printf("send  data #%d, winSize = %d\n",seq,winSize);
								maxHasSend = seq;
							}else{
								printf("rsend data #%d, winSize = %d\n",seq,winSize);
							}
							int k = write(conn_fd,packet,sizeof(char)*(len));
							//printf("k=%d\n",k);				
							hasSend[i] = 1;
							hasAck[i] = 0;
							fflush(stdout);
						}
					
						if(Buflen[i]<PKT_DATA_MAX_SIZE){
							lastSeq = seq + 1;
							//printf("find last Seq = %d\n",lastSeq);
							break;
						}
						seq++;
						close(conn_fd);
					}				
				}//end of for				
			}//end of if (send data)
					
			/* wait for ack */			
			select_num = select(max_fd_num,&reading_set,NULL,NULL,&timeout);
		
			if(select_num<0){
				//error
			}else if(select_num>0){
				
				if(FD_ISSET(listen_fd,&reading_set)){
			
					int k = recvfrom(listen_fd,ackpkt,sizeof(char)*PKT_ACK_SIZE,0,(struct sockaddr*)&clientAddress,&socklen);
				
					if(ackpkt[PKT_TYPE]==PKT_TYPE_ACK){	//ack
						int seqNum = getPktSeqNum(ackpkt);
						printf("recv  ack  #%d\n",seqNum);

						if(seqNum-base>=0)
							hasAck[seqNum-base] = 1;
					
						if(lastSeq<0){
							/* check if all ack is received */
							int allAck = 1;
							for(i=0;i<winSize;i++){
								if(hasAck[i]!=1){
									allAck = 0;
									break;
								}
							}
							/* if all ack is received, continue send data */
							if(allAck==1){
								base = seq;
								if(winSize<threshold){
									winSize = winSize*2;
								}else{
									winSize++;
								}
							}					
						}else{
							/* check if all ack is received */
							int allAck = 1;
							for(i=0;i<lastSeq-base;i++){
								if(hasAck[i]!=1){
									allAck = 0;
									break;
								}
							}
							/* if all ack is received, send fin */
							if(allAck==1){
								/* let send data stop */
								base = lastSeq-1;
								winSize = 0;
								/* send fin, make fin pkt */							
								len = makePktHeader(packet,"0.0.0.0",setting->send_port,setting->target_host,setting->target_port,PKT_TYPE_FIN,setting->agent_port,lastSeq,0);
								/* connect to target and send fin*/
								conn_fd = socket(AF_INET,SOCK_DGRAM,0);
								c = connect(conn_fd,(struct sockaddr *) &serverAddress,	sizeof(serverAddress));
								if(c==0){
									printf("send  fin\n");
									int k = write(conn_fd,packet,sizeof(char)*(len));
									//printf("k=%dlast=%dseq=%d\n",k,lastSeq,seq);
									//set local buf to wait ack fin
									hasSend[0] = 1;
									hasAck[0] = 0;
									fflush(stdout);
								}
								close(conn_fd);
							}						
						}
					}else if(ackpkt[PKT_TYPE]==PKT_TYPE_FIN){	//fin
						// if receive ack fin, finished.
						printf("recv  ack  fin\n");
						printf("Done.\n");
						break;
					}			
				}
			}else{
				/* Timeout */
				if(lastSeq<0){
					//check if all ack is received and find out which pkt have to resend
					int allAck = 1;				
					for(i=0;i<winSize;i++){
						if(hasAck[i]!=1){
							allAck = 0;
							break;
						}
					}
					if(allAck!=1){
						base = i + base;
						seq = base;
						threshold = winSize / 2;
						winSize = 1;
						printf("time  out, threshold = %d\n",threshold);
					}				
				}else{
					//check if all ack is received and find out which pkt have to resend
					int allAck = 1;				
					for(i=0;i<lastSeq-base;i++){
						if(hasAck[i]!=1){
							allAck = 0;
							break;
						}
					}
					if(allAck!=1){						
						base = i + base;
						seq = base;
						threshold = winSize / 2;
						winSize = 1;
						lastSeq = -1;
						printf("time  out, threshold = %d\n",threshold);
					}else{
						//all ack but timeout -> resend fin							
						len = makePktHeader(packet,"0.0.0.0",setting->send_port,setting->target_host,setting->target_port,PKT_TYPE_FIN,setting->agent_port,lastSeq,0);
						conn_fd = socket(AF_INET,SOCK_DGRAM,0);
						c = connect(conn_fd,(struct sockaddr *) &serverAddress,	sizeof(serverAddress));
						if(c==0){
							printf("rsend fin\n");
							int k = write(conn_fd,packet,sizeof(char)*(len));
							//printf("k=%d\n",k);
							hasSend[0] = 1;
							hasAck[0] = 0;
							fflush(stdout);
						}
						close(conn_fd);
					}
				}
			}//end of timeout
		}//end of while(1)	
	}else if(setting->recv==1){
		/* Be a Receiver */
		printf("Receiver (@%d)\n",setting->recv_port);
		
		int listen_fd;
		FILE *of;
		int output_fd;
		int conn_fd;
		int max_fd_num = getdtablesize();
		int select_num;
		int i,j,c;
		char ip[16];
		unsigned char inBuf[PKT_MAX_SIZE];
		unsigned char Buffer[32][PKT_DATA_MAX_SIZE];
		unsigned char ackpkt[PKT_ACK_SIZE];
		int ack_fd;
		int flush;
		int Buflen[32];
		char outName[1024];
		in_addr_t address;
		struct sockaddr_in serverAddress, clientAddress;
		socklen_t socklen = sizeof(clientAddress);
		struct timeval timeout;	// timeout
		fd_set reading_set;		// reading fd set
		
		/* Receiver's Server Setting */
		listen_fd = socket(AF_INET,SOCK_DGRAM,0);
		bzero(&serverAddress,sizeof(serverAddress));
		serverAddress.sin_family = AF_INET;
		serverAddress.sin_port   = htons(setting->recv_port);
		serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
		bind(listen_fd, (struct sockaddr *)&serverAddress,sizeof(serverAddress));
		listen(listen_fd,1);
		
		/* Buffer init */
		flush = 0;
		for(i=0;i<32;i++){
			Buflen[i] = -1;
		}
		
		/* Receiver main loop */
		while(1){
			/* Select setting */
			FD_ZERO(&reading_set);
			FD_SET(listen_fd,&reading_set);
			timeout.tv_sec = 10;
			timeout.tv_usec = 0;
			
			select_num = select(max_fd_num,&reading_set,NULL,NULL,&timeout);
			
			if(select_num < 0){
				//error
			}else if(select_num > 0){						
			
				if(FD_ISSET(listen_fd,&reading_set)){
						
					int k = recvfrom(listen_fd,inBuf,sizeof(char)*PKT_MAX_SIZE,0,(struct sockaddr*)&clientAddress,&socklen);
				
					/* get some infomation from pkt */
					int sendPort = getPktSendPort(inBuf);
					int seqNum = getPktSeqNum(inBuf);
				
					if(k>=PKT_DATA_MIN_SIZE){
						int dataLen = getPktDataLen(inBuf);
				
						int ignr = 0;
						if(flush*32>seqNum){
							//seqNum already flush, so ignore it.
							printf("recv  data #%d\n",seqNum);
							printf("ignr  data #%d\n",seqNum);
							ignr = 1;
							//continue;
						}else if(Buflen[seqNum%32]>0&&seqNum<(flush+1)*32){
							//now already in buffer, so ignore it.
							printf("recv  data #%d\n",seqNum);
							printf("ignr  data #%d\n",seqNum);
							ignr = 1;
							//continue;
						}else{
							//check is buffer full
							int fu = 0;
							for(i=0;i<32;i++){
								if(Buflen[i]>0){
									fu++;
								}
							}
							if(fu==32){
								//drop
								printf("drop  data #%d\n",seqNum);
								//flush
								if(flush==0){
									//first time flush
									if(setting->rename==1){
										of = fopen(setting->outputFilename,"w");
										output_fd = fileno(of);
									}else{
										Buffer[0][Buflen[0]]='\0';
										of = fopen(Buffer[0],"w");
										output_fd = fileno(of);
									}
									for(i=1;i<32;i++){
										write(output_fd,Buffer[i],Buflen[i]);
									}
								}else{
									for(i=0;i<32;i++){
										write(output_fd,Buffer[i],Buflen[i]);
									}
								}							
								//re init buf
								for(i=0;i<32;i++){
									Buflen[i] = -1;
								}
								printf("flush\n");
								flush++;
								continue;
							}else{
								//buffer is not full, check if it is in the range [flush*32,(flush+1)*32)
								if(seqNum>=(flush+1)*32){
									printf("drop  data #%d (out of order!) \n",seqNum);
									continue;
								}
							}
						}
					
						//accept pkt
						if(ignr==0){
							printf("recv  data #%d\n",seqNum);		
							//write into buf
							for(j=0;j<dataLen;j++){
								Buffer[seqNum%32][j] = inBuf[PKT_DATA_MIN_SIZE+j];
							}
							Buflen[seqNum%32] = dataLen;
							//printf("->%d\n",(seqNum+32)%32);
						}				
						
						//send ack
						printf("send  ack  #%d\n",seqNum);
						k = makePktHeader(ackpkt,&inBuf[PKT_RECV_IP],getPktRecvPort(inBuf),&inBuf[PKT_SEND_IP],sendPort,PKT_TYPE_ACK,0,seqNum,0);
						ack_fd = socket(AF_INET,SOCK_DGRAM,0);
						if(strcmp(&inBuf[PKT_SEND_IP],"0.0.0.0")==0){
							//printf("direct from sender!\n");
							clientAddress.sin_port = htons(sendPort);
						}else if(getPktAgentPort(inBuf)!=0){
							clientAddress.sin_port = htons(getPktAgentPort(inBuf));
						}
						c = connect(ack_fd,(struct sockaddr *) &clientAddress,sizeof(clientAddress));
						//printf("=:%d\n",ntohs(clientAddress.sin_port));
						if(c==0){
							k = write(ack_fd,ackpkt,sizeof(char)*(k));
							//printf("k=%d\n",k);
						}					
						close(ack_fd);
					}else{
						if(inBuf[PKT_TYPE]==PKT_TYPE_FIN){	//Fin
							printf("recv  fin\n");
							//prepare to send ack fin
							k = makePktHeader(ackpkt,&inBuf[PKT_RECV_IP],getPktRecvPort(inBuf),&inBuf[PKT_SEND_IP],getPktSendPort(inBuf),PKT_TYPE_FIN,0,seqNum,0);
							ack_fd = socket(AF_INET,SOCK_DGRAM,0);
							if(strcmp(&inBuf[PKT_SEND_IP],"0.0.0.0")==0){
								//printf("direct from sender!(fin)\n");
								clientAddress.sin_port = htons(sendPort);
							}else if(getPktAgentPort(inBuf)!=0){
								clientAddress.sin_port = htons(getPktAgentPort(inBuf));
							}
							printf("send  ack  fin\n");
							c = connect(ack_fd,(struct sockaddr *) &clientAddress,sizeof(clientAddress));
							if(c==0){
								k = write(ack_fd,ackpkt,sizeof(char)*(k));
								//printf("k=%d\n",k);
							}
							close(ack_fd);
							//flush buf
							printf("flush\n");
							if(flush==0){
								//first time flush
								if(setting->rename==1){
									of = fopen(setting->outputFilename,"w");
									output_fd = fileno(of);
								}else{
									Buffer[0][Buflen[0]]='\0';
									of = fopen(Buffer[0],"w");
									output_fd = fileno(of);
								}
								for(i=1;i<32;i++){
									if(Buflen[i]>0)
										write(output_fd,Buffer[i],Buflen[i]);
								}
							}else{
								for(i=0;i<32;i++){
									if(Buflen[i]>0)
										write(output_fd,Buffer[i],Buflen[i]);
								}
							}
							//close file		
							close(output_fd);
							printf("Done.\n");
							break;
						}
					}
				fflush(stdout);
				}			
			}else{
				//timeout
				//printf(".\n");	//for debug
			}
		}//end of while(1)
	}else{
		/* Error */
		printf("Setting error\n");
	}
	return 0;
}

