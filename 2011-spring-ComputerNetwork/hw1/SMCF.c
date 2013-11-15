/*
	Computer Network 2011 Homework 1
	b97501046 Qing-Cheng Li

	SimpleMail
	SMCF.c - Simple Mail Client Function
*/

#include "SMCF.h"

void welcomeMsg(){
		
	/* welcome msg */
	printf("Simple Mail Client (CN2011HW#1)\n");
	
}

void getSender(char *sender){
	
	printf("Please input Sender's email:");
	scanf("%s",sender);
	
}

int getReciver(char **reciver){
	
	int c_to = 0;
	char buf[1024];
	char *tmp;
	
	do{
		printf("Please input Reciver's email:");
		scanf("%s",buf);
		tmp = malloc(sizeof(char)*1024);
		strcpy(tmp, buf);
		reciver[c_to] = tmp;
		c_to++;
		printf("Add reciver? (y/n)");
		scanf("%s",buf);
	
	}while(buf[0]!='n'&&buf[0]!='N'&&c_to<=1024);
	
	return c_to;
}

int getCc(char **cc){
	
	int c_cc = 0;
	char buf[1024];
	char *tmp;
	
	while(1){
		printf("Add carbon copy? (y/n)");
		scanf("%s",buf);
		if(buf[0]=='n'||buf[0]=='N'&&c_cc<=1024){
			break;
		}
		printf("Please input Cc's email:");
		scanf("%s",buf);
		tmp = malloc(sizeof(char)*1024);
		strcpy(tmp, buf);
		cc[c_cc] = tmp;
		c_cc++;
	}
	
	return c_cc;
}

int getBcc(char **bcc){
	
	int c_bcc = 0;
	char buf[1024];
	char *tmp;
	
	while(1){
		printf("Add bline carbon copy? (y/n)");
		scanf("%s",buf);
		if(buf[0]=='n'||buf[0]=='N'&&c_bcc<=1024){
			break;
		}
		printf("Please input Bcc's email:");
		scanf("%s",buf);
		tmp = malloc(sizeof(char)*1024);
		strcpy(tmp, buf);
		bcc[c_bcc] = tmp;
		c_bcc++;
	
	}
	
	return c_bcc;
}

void getSubject(char *subject){
	
	printf("Please input Subject:");
	scanf("\n%[^\n]",subject);
	
}

void getMailbody(char *mailbody){
	
	printf("Please input Mailbody file:");
	scanf("%s",mailbody);
	
}

int getAttachment(char **file){
	
	int c_file = 0;
	char buf[1024];
	char *tmp;
	
	
	while(1){
		printf("Add attachment? (y/n)");
		scanf("%s",buf);
		if(buf[0]=='n'||buf[0]=='N'&&c_file<=1024){
			break;
		}
		printf("Please input filename:");
		scanf("%s",buf);
		tmp = malloc(sizeof(char)*1024);
		strcpy(tmp, buf);
		file[c_file] = tmp;
		c_file++;
	
	}
	
	return c_file;
}

int checkMail(
	char *from,
	char **to,int c_to,
	char **cc,int c_cc,
	char **bcc,int c_bcc,
	char *subject,
	char *mailbody,
	char **file,int c_file
){
	
	char buf[1024];
	int i;
	
	printf("\nYour mail:\n");
	printf("From:\n\t%s\n",from);
	printf("To:\n");
	for(i=0;i<c_to;i++){
		printf("\t%s\n",to[i]);
	}
	printf("Cc:\n");
	if(c_cc==0){
		printf("\tN\\A\n");
	}else{
		for(i=0;i<c_cc;i++){
			printf("\t%s\n",cc[i]);
		}
	}
	printf("Bcc:\n");
	if(c_cc==0){
		printf("\tN\\A\n");
	}else{
		for(i=0;i<c_bcc;i++){
			printf("\t%s\n",bcc[i]);
		}
	}
	printf("Subject:\n\t%s\n",subject);
	printf("Mailbody:\n\t%s\n",mailbody);
	printf("Attachment:\n");
	if(c_file==0){
		printf("\tN\\A\n");
	}else{
		for(i=0;i<c_file;i++){
			printf("\t%s\n",file[i]);
		}
	}
	
	while(1){
		printf("\n[S]end it, [R]eset or [Q]uit this program? (s/r/q):");
		scanf("%s",buf);
		switch(buf[0]){
			case 'S':
			case 's':
				return 1;
			case 'Q':
			case 'q':
				return -1;
			case 'R':
			case 'r':
				return 0;
		}
	}
	return -1; 	
}

int connectToHost(char *host, char *port, char *addr){
	
	int sockfd;
	int n;
	char buf[1024];
	struct sockaddr_in serverAddress;
	struct hostent *targetHost;
	
	//printf("host:%s\nport:%d\n",host,atoi(port));
	
	/* open socket and initalize */
	sockfd = socket(AF_INET,SOCK_STREAM, 0);
	bzero(&serverAddress, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(atoi(port));
	
	/* Get the ip of host */
	targetHost = gethostbyname(host);
	inet_ntop(AF_INET,targetHost->h_addr,addr,sizeof(char)*16);
	inet_pton(AF_INET,addr,&serverAddress.sin_addr);
	
	/* connect */
	int c = connect(sockfd,(struct sockaddr *) &serverAddress, sizeof(serverAddress));
	//printf("c=%d\n",c);
	
	if(c==0){
		n = read(sockfd,buf,sizeof(buf));
		if(n>=3){
			if(buf[0]=='2'&&buf[1]=='2'&&buf[2]=='0'){	//220
				return sockfd;
			}else{
				printf("Connect ERROR:%s\n",buf);
				return -1;
			}
		}else{
		    return -1;
		}		
	}else{
		return -1;
	}
}

int HELO(int fd){

	int n;
	char buf[1024];
	
	n = write(fd,"HELO b97501046\r\n",16);
	n = read(fd,buf,sizeof(buf));
	
	if(n>=3&&buf[0]=='2'&&buf[1]=='5'&&buf[2]=='0')	//250:ok
		return 0;
	
	printf("HELO ERROR:%s\n",buf);
	return -1;
}

int MAIL(int fd,char *from){
	
	int n;
	char buf[1024];
	
	n = write(fd,"MAIL FROM:<",11);
	n = write(fd,from,strlen(from));
	n = write(fd,">\r\n",3);
	n = read(fd,buf,sizeof(buf));
	
	if(n>=3&&buf[0]=='2'&&buf[1]=='5'&&buf[2]=='0')	//250:ok
		return 0;
	
	printf("MAIL ERROR:%s\n(mail from:%s)\n",buf,from);
	return -1;
	
}

int RCPT(int fd,char **to,int c_to,char **cc, int c_cc, char **bcc, int c_bcc){

	int n;
	int i;
	char buf[1024];

	for(i=0;i<c_to;i++){
		n = write(fd,"RCPT TO:<",9);
		if(strcmp(to[i],"")==0)
			continue;
		n = write(fd,to[i],strlen(to[i]));
		n = write(fd,">\r\n",3);
		n = read(fd,buf,sizeof(buf));
		
		if(n>=3&&buf[0]=='2'&&buf[1]=='5'&&buf[2]=='0')	//250:ok
			continue;
		
		printf("RCPT ERROR:%s(rcpt to:%s)\n",buf,to[i]);
		return -1;
		
	}

	for(i=0;i<c_cc;i++){
		n = write(fd,"RCPT TO:<",9);
		if(strcmp(cc[i],"")==0)
			continue;
		n = write(fd,cc[i],strlen(cc[i]));
		n = write(fd,">\r\n",3);
		n = read(fd,buf,sizeof(buf));
		
		if(n>=3&&buf[0]=='2'&&buf[1]=='5'&&buf[2]=='0')	//250:ok
			continue;
	
		printf("RCPT ERROR:%s\n(rcpt to:%s)\n",buf,cc[i]);
		return -2;
		
	}

	for(i=0;i<c_bcc;i++){
		n = write(fd,"RCPT TO:<",9);
		if(strcmp(bcc[i],"")==0)
			continue;
		n = write(fd,bcc[i],strlen(bcc[i]));
		n = write(fd,">\r\n",3);
		n = read(fd,buf,sizeof(buf));
		
		if(n>=3&&buf[0]=='2'&&buf[1]=='5'&&buf[2]=='0')	//250:ok
			continue;
	
		printf("RCPT ERROR:%s\n(rcpt to:%s)\n",buf,bcc[i]);
		return -3;
	}

	return 0;
}

int MAIL_Headr(int fd,char *from,char **to, int c_to,char **cc,int c_cc,char **bcc,int c_bcc,char *subject,int c_file){

	int n;
	int i;
	char buf[1024];

	n = write(fd,"DATA\r\n",6);
	n = read(fd,buf,sizeof(buf));
	
	if(n>=3&&buf[0]=='3'&&buf[1]=='5'&&buf[2]=='4'){	//354
	}else{
		printf("DATA ERROR:%s\n",buf);
		return -1;
	}
	
	
	/* Mail Header */
	
	/* From */
	n = write(fd,"From: ",6);
	n = write(fd,from,strlen(from));
	n = write(fd,"\n",1);

	/* To */
	n = write(fd,"To: ",4);
	n = write(fd,to[0],strlen(to[0]));
	for(i=1;i<c_to;i++){
		if(strcmp(to[i],"")==0)
			continue;
		n = write(fd,",",1);
		n = write(fd,to[i],strlen(to[i]));
	}
	n = write(fd,"\n",1);
	
	/* Cc */
	if(c_cc){
		n = write(fd,"Cc: ",4);
		n = write(fd,cc[0],strlen(cc[0]));
		for(i=1;i<c_cc;i++){
			if(strcmp(cc[i],"")==0)
				continue;
			n = write(fd,",",1);
			n = write(fd,cc[i],strlen(cc[i]));
		}
		n = write(fd,"\n",1);
	}
	
	/* Bcc */
	if(c_bcc){
		n = write(fd,"Bcc: ",5);
		n = write(fd,bcc[0],strlen(bcc[0]));
		for(i=1;i<c_bcc;i++){
			if(strcmp(bcc[i],"")==0)
				continue;
			n = write(fd,",",1);
			n = write(fd,bcc[i],strlen(bcc[i]));
		}
		n = write(fd,"\n",1);
	}
	
	/* Subject */
	n = write(fd,"Subject: ",9);
	n = write(fd,subject,strlen(subject));
	n = write(fd,"\n",1);
	
	/* MIME Header */
	n = write(fd,"MIME-Version: 1.0\n",18);
	
	if(c_file){
		n = write(fd,"Content-type: multipart/mixed; boundary=\"BOUNDARY\"\n\n",52);
	}
	//n = write(fd,"CN2011HW1\n",10);
	
	return 0;
}

int MAIL_body(int fd,char *mailbody,int c_file){

	int n;
	int in_fd;
	char buf[1024];
	
	if(c_file){
		n = write(fd,"--BOUNDARY\n",11);
	}

	n = write(fd,"Content-type: text/plain; charset=US-ASCII\n",43);
	n = write(fd,"Content-Transfer-Encoding: 7bit\n",32);

	/* HERE READ THE MAIL AND WRITE OUT */
	in_fd = open(mailbody,O_RDONLY);
	if(in_fd!=-1){
		while(1){
			bzero(buf,sizeof(buf));
			n = read(in_fd,buf,1024);
			if(n<=0){
				break;
			}else{
				n = write(fd,buf,n);
			}
		}
	}else{
		printf("File ``%s'' open fail\n",mailbody);
		n = write(fd,"Mailbody file open fail\n",24);
	}

	return 0;
}

int Attachment(int fd,char **file,int c_file){

	int n;
	int i;
	int j;
	int in_fd;
	int l;
	int count;
	unsigned char in_buf[3];
	unsigned char u[3];
	char e[4];
	char buf[1024];
	
	if(c_file){
		for(i=0;i<c_file;i++){
			if(strcmp(file[i],"")==0)
				continue;
			in_fd = open(file[i],O_RDONLY);
			if(in_fd!=-1){
				
				/*MIME HEADER*/
				n = write(fd,"--BOUNDARY\n",11);
				n = write(fd,"Content-type: application/x-msdownload; name=\"",46);
				n = write(fd,file[i],strlen(file[i]));
				n = write(fd,"\"\nContent-Transfer-Encoding: base64\n",36);
				n = write(fd,"Content-Disposition: attachment; filename=\"",43);
				n = write(fd,file[i],strlen(file[i]));
				n = write(fd,"\"\n\n",3);
				
				/*BASE64 ENCODING*/

				count = 0;
				while(1){
					bzero(in_buf,sizeof(in_buf));
					l = read(in_fd,in_buf,3);
					u[0] = in_buf[0];
					u[1] = in_buf[1];
					u[2] = in_buf[2];
					//u[0] = u[0] % 256;
					//u[1] = u[1] % 256;
					//u[2] = u[2] % 256;
					if(l==0){
						break;
					}else if(l==1){
						e[0] = base64encode(u[0] >> 2);
						e[1] = base64encode((u[0] & 3) << 4);
						e[2] = '=';
						e[3] = '=';
					}else if(l==2){
						e[0] = base64encode(u[0] >> 2);
						e[1] = base64encode(((u[0] & 3) << 4) + ((u[1]&240 ) >> 4));
						e[2] = base64encode((u[1] & 15) << 2);
						e[3] = '=';
					}else{
						e[0] = base64encode(u[0] >> 2);
						e[1] = base64encode(((u[0]&3)<<4)+((u[1]&240)>>4));
						e[2] = base64encode(((u[1]&15)<<2)+((u[2]&192)>>6));
						e[3] = base64encode(u[2]&63);
					}
					
					n = write(fd,e,4);
					count++;
					//printf(" => %c%c%c%c\n",e[0],e[1],e[2],e[3]);
					if(count==19){
						n = write(fd,"\n",1);
						count = 0; 	
					}
				}
				n = write(fd,"\r\n",2);
			}else{
				printf("File ``%s'' open fail\n",file[i]);	
			}
		}
	}

	return 0;
}

int EOM(int fd,int c_file){

	int n;
	char buf[1024];
	
	if(c_file){
		n = write(fd,"--BOUNDARY--\n",13);
	}
	
	n = write(fd,"\r\n.\r\n",5);
	n = read(fd,buf,sizeof(buf));
	
	if(n>=3&&buf[0]=='2'&&buf[1]=='5'&&buf[2]=='0')	//250:ok
		return 0;
	
	printf("Send mail ERROR:%s\n",buf);
	return -1;

}

int QUIT(int fd){

	int n;
	char buf[1024];

	n = write(fd,"QUIT\r\n",6);
	n = read(fd,buf,sizeof(buf));

	if(n>=3&&buf[0]=='2'&&buf[1]=='2'&&buf[2]=='1')	//221: service close
		return 0;
	
	printf("QUIT ERROR:%s\n",buf);
	return -1;

}

char base64encode(unsigned char a){
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

