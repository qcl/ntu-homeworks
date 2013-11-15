/*
	Computer Network 2011 Homework 3
	Ping Talk

	b97501046 Qing-Cheng Li
	cnping.h - header file of `Ping' object.
*/
#ifndef _CNPING_H_
#define _CNPING_H_

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <pthread.h>
#include <fcntl.h>

using namespace std;

class Ping{
	private:
		int icmp_socket;
		int ping_rcvskt;
		int buf_size;
		int req_num;
		int cur_seq;
		int ttl;
		int mode;
		char *target;
		u_char outpkt[8+1024];	//8 byte icmphdr
		u_short pid;
		struct icmphdr *icmpkt;
		struct sockaddr_in sourceAddr;
		struct sockaddr_in targetAddr;
		//record send time for compute rtt, prevent
		//user set bufsize=0
		struct timeval rtt_rcd[1024];
		
		/* ping */
		int send_pkt();
		int recv_pkt();
		void ping_main_loop();
		friend void* ppsp(void*);

		/* pingtalk */
		bool quit;
		int pingtalk_send_pkt();
		int pingtalk_recv_pkt();
		void pingtalk_main_loop();
		friend void* pppsp(void*);
		struct icmpdata{
			u_char type;
			u_char encode;
			u_short from;
			u_short len;
			u_short flag;
			unsigned int offset;
		};

		/* encoding & decoding */
		u_char b64en(u_char);
		u_char b64de(u_char);
	
	public:
		Ping();
		~Ping();

		void usage();
		bool init(int,char**);
		bool setting();
		void main_loop();

		static u_short checksum(u_short*,int,u_short);
		int base64encode(u_char*,int,u_char*);
		int base64decode(u_char*,int,u_char*);

};

#endif
