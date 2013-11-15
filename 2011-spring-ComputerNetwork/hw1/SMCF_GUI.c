/*
	Computer Network 2011 Homework 1
	b97501046 Qing-Cheng Li

	SimpleMail
	SMCF_GUI.c - Simple Mail Client GUI Function
*/
#include "SMCF_GUI.h"

void addTo(GtkWidget* widget,struct GUI *gui){
	if(gui->c_to>=1024)
		return;
	gui->iptTo[gui->c_to++] = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(gui->tableTo),gui->iptTo[gui->c_to-1],0,1,gui->c_to-1,gui->c_to);
	gtk_widget_show_all(gui->window);
}

void addCc(GtkWidget* widget,struct GUI *gui){
	if(gui->c_cc>=1024)
		return;
	gui->iptCc[gui->c_cc++] = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(gui->tableCc),gui->iptCc[gui->c_cc-1],0,1,gui->c_cc-1,gui->c_cc);
	gtk_widget_show_all(gui->window);
}

void addBcc(GtkWidget* widget,struct GUI *gui){
	if(gui->c_bcc>=1024)
		return;
	gui->iptBcc[gui->c_bcc++] = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(gui->tableBcc),gui->iptBcc[gui->c_bcc-1],0,1,gui->c_bcc-1,gui->c_bcc);
	gtk_widget_show_all(gui->window);
}

void addFile(GtkWidget* widget,struct GUI *gui){
	if(gui->c_file>=1024)
		return;
	gui->btnOpenfile[gui->c_file++] = gtk_file_chooser_button_new("Open file",GTK_FILE_CHOOSER_ACTION_OPEN);
	gtk_table_attach_defaults(GTK_TABLE(gui->tableFile),gui->btnOpenfile[gui->c_file-1],0,1,gui->c_file-1,gui->c_file);
	gtk_widget_show_all(gui->window);
}

void sendMail(GtkWidget* widget,struct GUI *gui){
	
	char addr[16];
	int sockfd;
	int n,i;
	
	/* Reset Statu Label */
	for(i=0;i<8;i++){
		gtk_label_set_text(GTK_LABEL(gui->statu[i]),"-");
	}
	
	/* Get host and port */
	gui->host = (char *)gtk_entry_get_text(GTK_ENTRY(gui->iptHost));
	gui->port = (char *)gtk_entry_get_text(GTK_ENTRY(gui->iptPort));
	
	/* Get From */
	gui->from = (char *)gtk_entry_get_text(GTK_ENTRY(gui->iptFrom));
	
	/* Get To */
	for(i=0;i<gui->c_to;i++){
		gui->to[i] = (char *)gtk_entry_get_text(GTK_ENTRY(gui->iptTo[i]));
	}
	
	/* Get Cc */
	for(i=0;i<gui->c_cc;i++){
		gui->cc[i] = (char *)gtk_entry_get_text(GTK_ENTRY(gui->iptCc[i]));
	}
	
	/* Get Bcc */
	for(i=0;i<gui->c_bcc;i++){
		gui->bcc[i] = (char *)gtk_entry_get_text(GTK_ENTRY(gui->iptBcc[i]));
	}
	
	/* Get Subject */
	gui->subject = (char *)gtk_entry_get_text(GTK_ENTRY(gui->iptSubject));
	
	/* Get Mailbody file */
	gui->mailbody = (char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gui->btnOpenmail));
	
	/* Get Attachment files */
	for(i=0;i<gui->c_file;i++){
		gui->file[i] = (char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(gui->btnOpenfile[i]));
	}
	
	if(strcmp(gui->host,"")==0||strcmp(gui->port,"")==0){
		gtk_label_set_text(GTK_LABEL(gui->statu[0]),"X");
		gtk_widget_show_all(gui->window);
		printf("host or port is (are) null string\n");
		return;
	}
	
	/* Connect to mail server */
	sockfd = connectToHost(gui->host,gui->port,addr);
	if(sockfd==-1){
		/* Connect Fail */
		printf("Connect to host %s:%s fail\n",gui->host,gui->port);
		gtk_label_set_text(GTK_LABEL(gui->statu[0]),"X");
		gtk_widget_show_all(gui->window);
		return;
	}else{
		printf("Connect to host %s(%s):%s\n",gui->host,addr,gui->port);
		
		n = HELO(sockfd);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			gtk_label_set_text(GTK_LABEL(gui->statu[0]),"X");
			gtk_widget_show_all(gui->window);
			return;
		}
		gtk_label_set_text(GTK_LABEL(gui->statu[0]),"O");
		gtk_widget_show_all(gui->window);
		
		
		n = MAIL(sockfd,gui->from);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			gtk_label_set_text(GTK_LABEL(gui->statu[1]),"X");
			gtk_widget_show_all(gui->window);
			return;
		}
		gtk_label_set_text(GTK_LABEL(gui->statu[1]),"O");
		gtk_widget_show_all(gui->window);
		
		
		n = RCPT(sockfd,gui->to,gui->c_to,gui->cc,gui->c_cc,gui->bcc,gui->c_bcc);
		switch(n){
			case -1:
				close(sockfd);
				printf("Close SimpleMail.\n");
				gtk_label_set_text(GTK_LABEL(gui->statu[2]),"X");
				gtk_widget_show_all(gui->window);
				return;
			case -2:
				close(sockfd);
				printf("Close SimpleMail.\n");
				gtk_label_set_text(GTK_LABEL(gui->statu[2]),"O");
				gtk_label_set_text(GTK_LABEL(gui->statu[3]),"X");
				gtk_widget_show_all(gui->window);
				return;
			case -3:
				close(sockfd);
				printf("Close SimpleMail.\n");
				gtk_label_set_text(GTK_LABEL(gui->statu[2]),"O");
				gtk_label_set_text(GTK_LABEL(gui->statu[3]),"O");
				gtk_label_set_text(GTK_LABEL(gui->statu[4]),"X");
				gtk_widget_show_all(gui->window);
				return;	
		}
		gtk_label_set_text(GTK_LABEL(gui->statu[2]),"O");
		gtk_label_set_text(GTK_LABEL(gui->statu[3]),"O");
		gtk_label_set_text(GTK_LABEL(gui->statu[4]),"O");
		gtk_widget_show_all(gui->window);
		
		
		n = MAIL_Headr(sockfd,gui->from,gui->to,gui->c_to,gui->cc,gui->c_cc,gui->bcc,gui->c_bcc,gui->subject,gui->c_file);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			gtk_label_set_text(GTK_LABEL(gui->statu[5]),"X");
			gtk_widget_show_all(gui->window);
			return;
		}
		gtk_label_set_text(GTK_LABEL(gui->statu[5]),"O");
		gtk_widget_show_all(gui->window);
		
		
		MAIL_body(sockfd,gui->mailbody,gui->c_file);
		Attachment(sockfd,gui->file,gui->c_file);
		gtk_label_set_text(GTK_LABEL(gui->statu[6]),"O");
		
		
		n = EOM(sockfd,gui->c_file);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			gtk_label_set_text(GTK_LABEL(gui->statu[7]),"X");
			gtk_widget_show_all(gui->window);
			return;
		}
		gtk_label_set_text(GTK_LABEL(gui->statu[7]),"O");
		gtk_widget_show_all(gui->window);
		
		
		n = QUIT(sockfd);
		if(n==-1){
			close(sockfd);
			printf("Close SimpleMail.\n");
			return;
		}
		
		printf("Send mail.\n");
		close(sockfd);	
	}
	
}

void initGui(struct GUI *gui){
	
	int i;
	
	gui->c_to = 0;
	gui->c_cc = 0;
	gui->c_bcc = 0;
	gui->c_file = 0;
	
	/* window */
	gui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(gui->window),"SimpleMail");
	g_signal_connect(GTK_OBJECT(gui->window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	
	/* entry */
	gui->iptFrom = gtk_entry_new();
	gui->iptSubject = gtk_entry_new();
	gui->iptPort = gtk_entry_new();
	gui->iptHost = gtk_entry_new();
	
	
	/* add button */
	gui->btnAddTo = gtk_button_new_with_label("+");
	gui->btnAddCc = gtk_button_new_with_label("+");
	gui->btnAddBcc = gtk_button_new_with_label("+");
	gui->btnAddFile = gtk_button_new_with_label("+");
	gui->btnSend = gtk_button_new_with_label("Send");
	
	/* open file button */
	gui->btnOpenmail = gtk_file_chooser_button_new("Open mail",GTK_FILE_CHOOSER_ACTION_OPEN);
	
	/* connect */
	g_signal_connect(GTK_OBJECT(gui->btnAddTo),"clicked",G_CALLBACK(addTo),gui);
	g_signal_connect(GTK_OBJECT(gui->btnAddCc),"clicked",G_CALLBACK(addCc),gui);
	g_signal_connect(GTK_OBJECT(gui->btnAddBcc),"clicked",G_CALLBACK(addBcc),gui);
	g_signal_connect(GTK_OBJECT(gui->btnAddFile),"clicked",G_CALLBACK(addFile),gui);
	g_signal_connect(GTK_OBJECT(gui->btnSend),"clicked",G_CALLBACK(sendMail),gui);
	
	/* table */
	gui->table = gtk_table_new(9,4,FALSE);
	gui->tableTo = gtk_table_new(1024,1,FALSE);
	gui->tableCc = gtk_table_new(1024,1,FALSE);
	gui->tableBcc = gtk_table_new(1024,1,FALSE);
	gui->tableFile = gtk_table_new(1024,1,FALSE);
	gui->tableCon = gtk_table_new(1,5,FALSE);
	
	//0-1
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gtk_label_new("Host:Port"),0,1,0,1);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gtk_label_new("From:"),0,1,1,2);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gtk_label_new("To:"),0,1,2,3);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gtk_label_new("Cc:"),0,1,3,4);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gtk_label_new("Bcc:"),0,1,4,5);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gtk_label_new("Subject:"),0,1,5,6);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gtk_label_new("Mail body:"),0,1,6,7);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gtk_label_new("Attachment:"),0,1,7,8);
	
	//1-2
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->tableCon,1,3,0,1);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->iptFrom,1,2,1,2);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->tableTo,1,2,2,3);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->tableCc,1,2,3,4);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->tableBcc,1,2,4,5);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->iptSubject,1,2,5,6);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->btnOpenmail,1,2,6,7);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->tableFile,1,2,7,8);

	//2-3
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->btnAddTo,2,3,2,3);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->btnAddCc,2,3,3,4);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->btnAddBcc,2,3,4,5);
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->btnAddFile,2,3,7,8);
	
	//3-4
	for(i=0;i<8;i++){
		gui->statu[i] = gtk_label_new("-");
		gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->statu[i],3,4,i,i+1);
	}
	gtk_table_attach_defaults(GTK_TABLE(gui->table),gui->btnSend,3,4,8,9);
		
	/* init iptTo */
	gui->iptTo[gui->c_to++] = gtk_entry_new();
	gtk_table_attach_defaults(GTK_TABLE(gui->tableTo),gui->iptTo[gui->c_to-1],0,1,gui->c_to-1,gui->c_to);
	
	/* init tableCon*/
	gtk_table_attach_defaults(GTK_TABLE(gui->tableCon),gui->iptHost,0,3,0,1);
	gtk_table_attach_defaults(GTK_TABLE(gui->tableCon),gtk_label_new(":"),3,4,0,1);
	gtk_table_attach_defaults(GTK_TABLE(gui->tableCon),gui->iptPort,4,5,0,1);
	
	
	gtk_container_add(GTK_CONTAINER(gui->window), gui->table);
	
}

