/*
	Computer Network 2011 Homework 3
	Ping Talk

	b97501046 Qing-Cheng Li
	cnping.cc - implementation of the object Ping that 
	defined in cnping.h.
*/

#include "cnping.h"

Ping::Ping(){
	cout << "B97501046 CNHW#3 PingTalk" << endl;
	/* set default value */
	req_num = 10;
	buf_size = 56;
	ttl = 20;
	mode = 0;
	cur_seq = 0;
	pid = getpid()&0xFFFF;
	quit = false;
	bzero(&sourceAddr,sizeof(sourceAddr));
	bzero(&targetAddr,sizeof(targetAddr));

}

Ping::~Ping(){

}

void Ping::usage(){
	cout << "ping <host or IP>";
	cout << " -c <number of request>" << endl;
	cout << " -s <size of buffer>";
	cout << " -t <TTL>";
	cout << "\n -m <mode>(pingtalk)";
	cout << "\nmode: (default=0)" << endl;
	cout << "\t0:ping" << endl;
	cout << "\t1:pingtalk" << endl;
}

bool Ping::init(int argc, char* argv[]){

	struct hostent *queryHost;

	if(argc==1||strcmp(argv[1],"-h")==0){
		usage();
		return false;
	}else{
		
		/* create icmp socket */
		icmp_socket = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
		if(icmp_socket<0){
			cerr << "err:icmp_socket are you superuser?" << endl;
			return false;
		}

		/* set target host */
		target = argv[1];
		targetAddr.sin_family = AF_INET;
		if(inet_aton(target,&targetAddr.sin_addr)==0){
			queryHost = gethostbyname(target);
			if(!queryHost){
				cerr << "err: host:" << target << endl;
				return false;
			}
			memcpy(&targetAddr.sin_addr, queryHost->h_addr, 4);
			target = (char*)malloc(sizeof(char)*16);
			inet_ntop(AF_INET,queryHost->h_addr,target,sizeof(char)*16);
		}
		targetAddr.sin_port = htons(1025);

		/* read other arg */
		for(int i=2;i<argc;i++){
			if(strcmp(argv[i],"-c")==0){	//req #
				if(i!=argc){
					req_num = atoi(argv[++i]);
					if(req_num<=0){
						cerr << "err: -c error" << endl;
						return false;
					}
				}else{
					cerr << "err: -c error" << endl;
					return false;
				}
			}else if(strcmp(argv[i],"-s")==0){	//buf size
				if(i!=argc){
					buf_size = atoi(argv[++i]);
					if(buf_size<0||buf_size>1024){
						cerr << "err: -s error" << endl;
						return false;
					}
				}else{
					cerr << "err: -s error" << endl;
					return false;
				}
			}else if(strcmp(argv[i],"-t")==0){	//ttl
				if(i!=argc){
					ttl = atoi(argv[++i]);
					if(ttl<=0){
						cerr << "err: -t error" << endl;
						return false;
					}
				}else{
					cerr << "err: -t error" << endl;
					return false;
				}			
			}else if(strcmp(argv[i],"-m")==0){	//mode
				if(i!=argc){
					mode = atoi(argv[++i]);
					if(mode<0||mode>1){
						cerr << "err: -m error" << endl;
						return false;
					}
					buf_size = 1024;
				}else{
					cerr << "err: -m error" << endl;
					return false;
				}
			}else{
				cerr << "err: arg error" << endl;
				return false;
			}
		}

		if(mode==0){
			cout << "Pinging " << argv[1] << "(" << target << ") with ";
			cout << buf_size << " bytes of data:" << endl;
		}

		return true;
	}
}

bool Ping::setting(){
	
	if(setsockopt(icmp_socket,IPPROTO_IP,IP_TTL,&ttl,sizeof(ttl))==-1){
		cerr << "set ttl error" << endl;
		return false;
	}

	return true;

}

/*
	proxy pthread send pkt function
*/
void* ppsp(void* ptr){
	static_cast<Ping*>(ptr)->send_pkt();
	pthread_exit(0);
}

int Ping::send_pkt(){

	int send_size;

	for(int i=0;i<req_num;i++){

		cur_seq = i;
		bzero(outpkt,sizeof(u_char)*(8+buf_size));

		/* make icmp pkt */
		icmpkt = (struct icmphdr *)outpkt;
		icmpkt->type = ICMP_ECHO;
		icmpkt->code = 0;
		icmpkt->checksum = 0;
		icmpkt->un.echo.sequence = htons(cur_seq);
		icmpkt->un.echo.id = pid;
	
		icmpkt->checksum = checksum((u_short*)icmpkt,8+buf_size,0);

		/* send */
		send_size = sendto(icmp_socket,icmpkt,8+buf_size,0,(struct sockaddr*)&targetAddr,sizeof(targetAddr));

		if(send_size==8+buf_size){
			gettimeofday(&rtt_rcd[cur_seq%1024],NULL);
		}else{
			cerr << "send_size error" << endl;
		}
		sleep(1);
	}

	return 0;
}

int Ping::recv_pkt(){

	int recv_size;
	bool skip;
	char recv_from[16];
	u_char inpkt[20+8+1024];
	u_short recv_seq;
	u_short check;
	struct sockaddr_in tmpAddr;
	struct timeval recv_time;
	struct iphdr *recviphd;
	struct icmphdr *recvpkt;
	socklen_t socklen = sizeof(tmpAddr);

	skip = false;
	recv_seq = 0;

	/* receive icmp pkt */
	recv_size = recvfrom(icmp_socket,inpkt,20+8+1024,0,(struct sockaddr*)&tmpAddr,&socklen);
	gettimeofday(&recv_time,NULL);	//record time for computing rtt
	inet_ntop(AF_INET,&tmpAddr.sin_addr,recv_from,sizeof(char)*16);	//where the pkt from.

	/* ip header of pkt */
	recviphd = (struct iphdr*) inpkt;
	if(recviphd->protocol!=IPPROTO_ICMP){	//check protocol
		//skip
		skip = true;
	}

	if(!skip){
		/* icmp header of pkt */
		recvpkt = (struct icmphdr*)(inpkt+recviphd->ihl*4);	//get icmp header
		recv_seq = ntohs(recvpkt->un.echo.sequence);	//get seq#
		check = recvpkt->checksum;	//get checksum
		recvpkt->checksum = 0;	//reset checksum for check checksum
		/* check checksum */
		if(check!=checksum((u_short*)recvpkt,recv_size-recviphd->ihl*4,0)){
			cout << "check sum error" << endl;
			skip = true;
		}
	}

	if(!skip){
		/* if it's a icmp echo pkt, skip it.	*/
		if(recvpkt->type==ICMP_ECHO){
			skip = true;
		}
	}

	if(!skip){
		
		cout << "Reply from " << recv_from << " : ";

		switch(recvpkt->type){
			case ICMP_ECHOREPLY:
				cout << "seq = " << recv_seq;
				cout << " byte = " << recv_size - recviphd->ihl*4 - sizeof(struct icmphdr);
				cout << " RTT = " << (double)((recv_time.tv_sec*1000 + recv_time.tv_usec) - (rtt_rcd[recv_seq%1024].tv_sec*1000 + rtt_rcd[recv_seq%1024].tv_usec))/(double)1000 << " msec" << endl;
				break;

			case ICMP_DEST_UNREACH:
				if(recvpkt->code==ICMP_NET_UNREACH){
					cout << "Destination network unreachable" << endl;
				}else if(recvpkt->code==ICMP_HOST_UNREACH){
					cout << "Destination host unreachable" << endl;
				}else{
					cout << "unknown icmp code" << endl;
				}
				break;

			case ICMP_TIME_EXCEEDED:
				if(recvpkt->code==ICMP_EXC_TTL){
					cout << "Time to live exceeded" << endl;
				}else{
					cout << "unknown icmp code" << endl;
				}
				break;

			default:
				cout << "unknown icmp type" << endl;
				break;
		}

	}
	
	return recv_seq;
}

void Ping::main_loop(){
	if(mode==0){	//ping mode
		ping_main_loop();	
	}else if(mode==1){
		pingtalk_main_loop();	//pingtalk mode
	}
}

void Ping::ping_main_loop(){
	
	int timeout_num;
	int select_num;
	int ack;
	int recv_seq;
	int max_fd_num = getdtablesize();
	fd_set reading_set;
	pthread_t ptid;
	struct timeval timeout;
	
	/* create new thread for send icmp echo pkt*/
	pthread_create(&ptid,NULL,ppsp,this);
	
	/* recv */
	timeout_num = ack = 0;
	while(1){
		FD_ZERO(&reading_set);
		FD_SET(icmp_socket,&reading_set);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		select_num = select(max_fd_num,&reading_set,NULL,NULL,&timeout);

		if(select_num<0){
			cerr << "err:select error" << endl;
			break;
		}else if(select_num>0){
			if(FD_ISSET(icmp_socket,&reading_set)){
				timeout_num = 0;
				recv_seq = recv_pkt();	//recv pkt
				if(recv_seq>=ack){
					ack = recv_seq;
				}
			}else{
				cerr << "err:select recv error" << endl;
			}
		}else{	//timeout
			timeout_num++;
			if(timeout_num>ttl){
				break;
			}else if(ack+1==req_num){
				break;
			}
		}

	}	

}

/*
	proxy pthread pingtalk send pkt function
*/
void* pppsp(void* ptr){
	static_cast<Ping*>(ptr)->pingtalk_send_pkt();
	pthread_exit(0);
}

int Ping::pingtalk_send_pkt(){

	int send_size;
	int read_size;
	int max_read_size;
	int input_fd;
	int type;
	int msgsize;
	int encode;
	int offset;
	int base;
	char text_msg[512];
	char filename[1024];
	char newfilename[1024];
	char ctr;
	char enc;
	u_char text_enc_msg[1024];
	u_char filedata[1024];
	u_char encfiledata[1024];
	u_short tid;
	bool error;
	struct icmpdata* icmpd;
	
	cur_seq = 0;

	while(1){

		/* get user input */
		error = false;
		cin >> ctr;
		if(ctr=='q'){	//quit pingtalk
			quit = true;
			cout << "Quit PingTalk..." << endl;
			break;
		}else{
			cin >> ws >> enc >> ws >> tid;
		
			switch(enc){	//encoding
				case 'n':	//no encoding
					encode = 0;
					break;
				case 'e':	//encoding (with base64)
					encode = 1;
					break;
				default:
					error = true;
					break;
			}

			if(ctr=='t'){	//text msg
				type = 0;
				cin >> text_msg;
			}else if(ctr=='f'){	//file
				type = 1;
				cin >> filename >> ws >> newfilename;
			}else{
				error = true;
			}

			if(error){
				cout << "input syntax error, please restart pingtalk..." << endl;
				quit = true;
				break;
			}
			
			/* make pkt */
			bzero(outpkt,sizeof(u_char)*(8+buf_size));
			icmpkt = (struct icmphdr*)outpkt;
			icmpkt->type = ICMP_ECHO;
			icmpkt->code = 0;
			icmpkt->un.echo.id = tid;
			
			icmpd = (struct icmpdata*)(outpkt+sizeof(struct icmphdr));
			icmpd->from = pid;
			icmpd->type = type;
			icmpd->encode = encode;
			icmpd->flag = 2;	//ok

			base = sizeof(struct icmphdr) + sizeof(struct icmpdata);

			if(type==0){	//text msg
			
				icmpkt->checksum = 0;
				icmpkt->un.echo.sequence = htons(cur_seq++);
				icmpd->offset = htonl(0);
				msgsize = strlen(text_msg);

				if(encode==0){	//no encoding
				
					icmpd->len = htons(msgsize);
					memcpy(outpkt+base,text_msg,msgsize);
								
				}else if(encode==1){	//encoding with base 64

					msgsize = base64encode((u_char*)text_msg,msgsize,text_enc_msg);
					icmpd->len = htons(msgsize);
					memcpy(outpkt+base,text_enc_msg,msgsize);
				}

				/* compute checksum */
				buf_size = sizeof(struct icmpdata)+msgsize;
				icmpkt->checksum = checksum((u_short*)icmpkt,8+buf_size,0);
				
				/* send pkt */
				send_size = sendto(icmp_socket,icmpkt,8+buf_size,0,(struct sockaddr*)&targetAddr,sizeof(targetAddr));

				if(send_size!=8+buf_size){
					cerr << "err: send err" << endl;
				}
			
			}else if(type==1){	//file
			
				/* check file */
				input_fd = open(filename,O_RDONLY);	//open file
				if(input_fd<0){
					cerr << "err: open file err" << endl;
				}else{

					/* 1st send newfilename */
					icmpkt->checksum = 0;
					icmpkt->un.echo.sequence = htons(cur_seq++);
					icmpd->offset = htonl(0);
					icmpd->flag = 0;	//filename
					msgsize = strlen(newfilename);
					if(encode==0){	//filename without encoding
						icmpd->len = htons(msgsize);
						memcpy(outpkt+base,newfilename,msgsize);
					}else if(encode==1){	//filename with base64 encoding
						msgsize = base64encode((u_char*)newfilename,msgsize,text_enc_msg);
						icmpd->len = htons(msgsize);
						memcpy(outpkt+base,text_enc_msg,msgsize);
					}
					buf_size = sizeof(struct icmpdata)+msgsize;
					icmpkt->checksum = checksum((u_short*)icmpkt,8+buf_size,0);
					//send
					send_size = sendto(icmp_socket,icmpkt,8+buf_size,0,(struct sockaddr*)&targetAddr,sizeof(targetAddr));
	
					if(send_size!=8+buf_size){
						cerr << "err: send filename err" << endl;
					}
					sleep(1);
	
					/* send file */
					if(encode==0){
						max_read_size = 1000;
					}else if(encode==1){
						max_read_size = 750;
					}
					offset = 0;
					lseek(input_fd,offset,SEEK_SET);
					while(read_size=read(input_fd,filedata,max_read_size)){
						icmpkt->checksum = 0;
						icmpkt->un.echo.sequence = htons(cur_seq++);
						msgsize = read_size;
						icmpd->offset = htonl(offset);
						icmpd->flag = 1;	//data
						if(encode==0){
							icmpd->len = htons(msgsize);
							memcpy(outpkt+base,filedata,msgsize);
						}else if(encode==1){
							msgsize = base64encode((u_char*)filedata,msgsize,encfiledata);
							icmpd->len = htons(msgsize);
							memcpy(outpkt+base,encfiledata,msgsize);
						}
						buf_size = sizeof(struct icmpdata)+msgsize;
						icmpkt->checksum = checksum((u_short*)icmpkt,8+buf_size,0);
						//send
						send_size = sendto(icmp_socket,icmpkt,8+buf_size,0,(struct sockaddr*)&targetAddr,sizeof(targetAddr));
	
						if(send_size!=8+buf_size){
							cerr << "err: send filename err" << endl;
						}

						offset+=read_size;
						lseek(input_fd,offset,SEEK_SET);
						sleep(1);
					}

					/* send fin */
					icmpkt->checksum = 0;
					icmpkt->un.echo.sequence = htons(cur_seq++);
					msgsize = 0;
					icmpd->offset = htonl(0);
					icmpd->flag = 2;	//fin
					icmpd->len = htons(0);
					buf_size = sizeof(struct icmpdata)+msgsize;
					icmpkt->checksum = checksum((u_short*)icmpkt,8+buf_size,0);
					//send
					send_size = sendto(icmp_socket,icmpkt,8+buf_size,0,(struct sockaddr*)&targetAddr,sizeof(targetAddr));
	
					if(send_size!=8+buf_size){
						cerr << "err: send filename err" << endl;
					}
				}
				//close file
				close(input_fd);

			}	

		}
	}
	return 0;
}

int Ping::pingtalk_recv_pkt(){

	int recv_size;
	int write_size;
	int decode_size;
	int select_num;
	int out_fd;
	int max_fd_num = getdtablesize();
	int pkt_data_base;
	bool skip;
	char recv_from[16];
	u_char recv_text[1024];
	u_char recv_dec_text[1024];
	u_char inpkt[20+8+1024];
	u_char data_type;
	u_char data_encode;
	u_short data_len;
	u_short data_from;
	u_short check;
	u_short recv_seq;
	u_short recv_id;
	u_short data_flag;
	fd_set reading_set;
	struct sockaddr_in tmpAddr;
	struct timeval recv_time;
	struct iphdr *recviphd;
	struct icmphdr *recvpkt;
	struct icmpdata *icmpd;
	struct timeval timeout;
	socklen_t socklen = sizeof(tmpAddr);
	unsigned int data_offset;
	FILE* of;


	/* recv */
	while(1){
		FD_ZERO(&reading_set);
		FD_SET(icmp_socket,&reading_set);
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		skip = false;
		select_num = select(max_fd_num,&reading_set,NULL,NULL,&timeout);

		if(select_num<0){
			cerr << "err:select error" << endl;
			break;
		}else if(select_num>0){
			if(FD_ISSET(icmp_socket,&reading_set)){
				/* receive icmp pkt */
				recv_size = recvfrom(icmp_socket,inpkt,20+8+1024,0,(struct sockaddr*)&tmpAddr,&socklen);
				inet_ntop(AF_INET,&tmpAddr.sin_addr,recv_from,sizeof(char)*16);
				
				/* ip header */
				recviphd = (struct iphdr*) inpkt;
				if(recviphd->protocol!=IPPROTO_ICMP){	//check protocol
					skip = true;
				}

				/* check checksum */
				if(!skip){
					recvpkt = (struct icmphdr*)(inpkt+recviphd->ihl*4);
					recv_seq = ntohs(recvpkt->un.echo.sequence);
					recv_id = recvpkt->un.echo.id;
					check = recvpkt->checksum;
					recvpkt->checksum = 0;
					if(check!=checksum((u_short*)recvpkt,recv_size-recviphd->ihl*4,0)){
						skip = true;
					}
				}

				pkt_data_base = sizeof(struct iphdr) + sizeof(struct icmphdr);
				
				if(!skip){
					
					icmpd = (struct icmpdata*)(inpkt+pkt_data_base);
					data_type = icmpd->type;
					data_encode = icmpd->encode;
					data_len = ntohs(icmpd->len);
					data_offset = ntohl(icmpd->offset);
					data_from = icmpd->from;
					data_flag = icmpd->flag;
					
					if(recv_id==pid&&recvpkt->type==ICMP_ECHO){	//data pkt
					
						cout << "Recv from " << recv_from << " seq=" << recv_seq << " ,`data' len=" << data_len << endl;;
						pkt_data_base += sizeof(struct icmpdata);

						if(data_type==0){	//text msg

							memcpy(recv_text,inpkt+pkt_data_base,data_len);
							if(data_encode==0){		//text msg without encoding
								recv_text[data_len] = '\0';
								cout << "From " << data_from << " : " << recv_text << endl;
							}else if(data_encode==1){	//text msg with base64 encoding
								decode_size = base64decode(recv_text,data_len,recv_dec_text);
								recv_dec_text[decode_size] = '\0';
								cout << "From " << data_from << " (encode msg) : " << recv_dec_text << endl;
							}
						
						}else if(data_type==1){	//file msg
							
							if(data_flag==0){	//filename
								
								memcpy(recv_text,inpkt+pkt_data_base,data_len);
								if(data_encode==0){
									recv_text[data_len] = '\0';
								}else if(data_encode==1){
									decode_size = base64decode(recv_text,data_len,recv_dec_text);
									memcpy(recv_text,recv_dec_text,decode_size);
									recv_text[decode_size] = '\0';
								}
								cout << "File " << recv_text << " from " << data_from << endl;
								//open file
								of = fopen((char*)recv_text,"w");
								out_fd = fileno(of);

							}else if(data_flag==1){	//file data
								
								lseek(out_fd,data_offset,SEEK_SET);
								memcpy(recv_text,inpkt+pkt_data_base,data_len);
								if(data_encode==0){
									//recv_text[data_len] = '\0';
								}else if(data_encode==1){
									decode_size = base64decode(recv_text,data_len,recv_dec_text);
									memcpy(recv_text,recv_dec_text,decode_size);
									//recv_text[decode_size] = '\0';
									data_len = decode_size;
								}
								cout << "Data offset=" << data_offset << ", size=" << data_len << endl;
								write_size = write(out_fd,recv_text,data_len);
								if(write_size!=data_len){
									cerr << "err: write error" << endl;
								}

							}else if(data_flag==2){	//fin
								cout << "Data from " << data_from << " is ok." << endl;
								//close file
								fclose(of);
							}						
						}

					}else if(data_from==pid&&recvpkt->type==ICMP_ECHOREPLY){	//ack pkt
						cout << "Ack " << recv_seq << endl;
						if(data_flag==2){
							cout << "OK" << endl;
						}
					}else{
						//ignore.
					}
				}

			}else{
				cerr << "err:select recv error" << endl;
			}
		}else{
			if(quit){
				break;
			}
		}
	}	
	return 0;
}

void Ping::pingtalk_main_loop(){

	pthread_t ptid;

	cout << "PingTalk mode" << endl;
	cout << "My ID=" << pid << endl;
	cout << "type `t {n|e} [receiver id] [text](no ws)' to send text msg" << endl;
	cout << "type `f {n|e} [receiver id] [file] [new name]' to send file" << endl;
	cout << "type `q' to quit this program" << endl;
	
	/* create new thread for send pkt*/
	pthread_create(&ptid,NULL,pppsp,this);

	/* recv pkt */
	pingtalk_recv_pkt();

}

u_short Ping::checksum(u_short *addr, int len, u_short sum){

	int tmpsum = sum;
	int i=0;
	u_short ans;

	while(len>1){
		tmpsum += addr[i++];
		len -= 2;
	}

	if(len==1){
		tmpsum += htons(addr[i]<<8);
	}

	tmpsum = (tmpsum>>16)+(tmpsum&0xFFFF);
	tmpsum += (tmpsum>>16);
	ans = ~tmpsum;

	return ans;

}

int Ping::base64encode(u_char* in,int in_len,u_char* out){
	
	int base;
	int ob;	//out base
	
	base = ob = 0;
	while(in_len>0){
		
		if(in_len>=3){
			out[ob] = b64en(in[base]>>2);
			out[ob+1] = b64en(((in[base]&3)<<4)+((in[base+1]&240)>>4));
			out[ob+2] = b64en(((in[base+1]&15)<<2)+((in[base+2]&192)>>6));
			out[ob+3] = b64en(in[base+2]&63);
			in_len-=3;
			base+=3;
		}else if(in_len==2){
			out[ob] = b64en(in[base]>>2);
			out[ob+1] = b64en(((in[base]&3)<<4)+((in[base+1]&240)>>4));
			out[ob+2] = b64en((in[base+1]&15)<<2);
			out[ob+3] = '=';
			in_len-=2;
		}else if(in_len==1){
			out[ob] = b64en(in[base]>>2);
			out[ob+1] = b64en((in[base]&3)<<4);
			out[ob+2] = '=';
			out[ob+3] = '=';
			in_len-=1;
		}
		ob+=4;
	
	}

	return ob;
}

u_char Ping::b64en(u_char a){
	if(a>=0&&a<=25){
		return 'A'+a;
	}else if(a>=26&&a<=51){
		return 'a'+(a-26);
	}else if(a>=52&&a<=61){
		return '0'+(a-52);
	}else if(a==62){
		return '+';
	}else{
		return '/';
	}
}

int Ping::base64decode(u_char* in,int in_len,u_char* out){

	int base;
	int ob;	//out base

	base = ob = 0;

	while(in_len>0){
		if(in[base+2]=='='){
			out[ob] = (b64de(in[base])<<2)+(b64de(in[base+1])>>4);
			ob+=1;
		}else if(in[base+3]=='='){
			out[ob] = (b64de(in[base])<<2)+(b64de(in[base+1])>>4);
			out[ob+1] = (b64de(in[base+1])<<4)+(b64de(in[base+2])>>2);
			ob+=2;
		}else{
			out[ob] = (b64de(in[base])<<2)+(b64de(in[base+1])>>4);
			out[ob+1] = (b64de(in[base+1])<<4)+(b64de(in[base+2])>>2);
			out[ob+2] = (b64de(in[base+2])<<6)+(b64de(in[base+3]));
			ob+=3;
		}
		in_len-=4;
		base+=4;
	}
	return ob;
}

u_char Ping::b64de(u_char a){
	if(a>='A'&&a<='Z'){
		return a-'A';	
	}else if(a>='a'&&a<='z'){
		return a-'a'+26;
	}else if(a>='0'&&a<='9'){
		return a-'0'+52;
	}else if(a=='+'){
		return 62;
	}else if(a=='/'){
		return 63;
	}
}

