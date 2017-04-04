/*
A simple echo server:

./server server_port_no   //port_no to which server is listening to

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define ERROR -1
#define MAX_CLIENTS 2
#define MAX_DATA 102400


//============================= Checks the connection of client ========================================

int check_client_type(char login[],int login_len,char password[],int password_len){

	FILE *fp;
	fp=fopen("login.txt","r");
	int i,j;
	char log[MAX_DATA],pass[MAX_DATA],typ[MAX_DATA];

	for(j=0;j<14;j++){
		fscanf(fp,"%s",log);
		log[strlen(log)]='\0';
		fscanf(fp,"%s",pass);
		pass[strlen(pass)]='\0';
		fscanf(fp,"%s",typ);
		int flag=0;
		//printf("%s %s %s     %s %s %d %d\n",log,pass,typ,login,password,strcmp(log,login),strcmp(password,pass));
		if(strcmp(log,login)==0 && strcmp(pass,password)==0 )
		flag=1;

		// for(i=0;line[i]!=" " || login[i]!='\0';i++)
		// 	if(login[i]!=line[i]) flag=1;
		if(flag==1){
			if(typ[0]=='C') return 1;
			if(typ[0]=='A') return 2;
			return 3;
		}


	}
	return -1;
}

int ase(char * data)
{  int i; 
   for(i=0;i<strlen(data);i++){
    //printf("char=%c\n",data[i]);
    if(data[i]-'0'<0 || data[i]-'9'>0)
        return 1;
   }
   return 0;

}

int con_len(char req[]){
    int count=0 ,i;
    for(i=0;req[i]!='\0' ;i++){
        //printf("char[]==%c\n",req[i]);
        if(req[i]=='\n') count++;
        if(count==3) break;
    }

    for(;req[i]!=' ';i++);
        char num[10];int j=0;
    i++;
    for(;req[i]!='\n';i++){
        
        num[j]=req[i];
        j++;
    }

    return atoi(num);


}







//==============================================================================================================================
void main(int argc, char **argv)
{
	
	struct sockaddr_in server;
	struct sockaddr_in client;
	int sock;    // socket for server
	int new;     // socket for client which may get changed with each new connection set with server
	
	int sockaddr_len;	
	sockaddr_len= sizeof(struct sockaddr_in);
	
	char data[MAX_DATA];
    char req[MAX_DATA];
    char res[MAX_DATA];
	int data_len,res_len,req_len;

	char login[MAX_DATA],password[MAX_DATA];
	int login_len,password_len;

    time_t t;
    time(&t);
    // checking for server socket, if not successful gives error  
	if((sock = socket(AF_INET, SOCK_STREAM,0))== ERROR) 
	{
		perror("server socket:");
		exit(-1);
	}

	server.sin_family=AF_INET;
	server.sin_port=htons(atoi(argv[1]));
	server.sin_addr.s_addr=INADDR_ANY; //INADDR_ANY =0.0.0.0, any address of interface attached to local machine 
	bzero(&server.sin_zero,8);
	
	// binding the above socket 'sock' with server sockaddr
	if( (bind(sock,(struct sockaddr *) &server, sockaddr_len))== ERROR)
	{
		perror("bind");
		exit(-1);
	}
    

    // server wait for clients to connect, a maximum of MAX_CLIENTS in the queue
START :	if((listen(sock,MAX_CLIENTS))==ERROR)
	{
		perror("listen");
		exit(-1);
	}
char ask[MAX_DATA]="Nyetr";
	while(ask[0]!='Y' && ask[0]!='y')  // this loop will continue until Ctrl+C is pressed on server side
	{
		// connected client is given socket new, if not successful gives error
		if((new=accept(sock,(struct sockaddr *) &client,&sockaddr_len))==ERROR)
		{
			perror("accept");
			exit(-1);
		}

        printf("New client connected from port no %d and IP %s\n",
        	    ntohs(client.sin_port),        // client port of 2 bytes(short) converted from network byte stuffing 
        	                                   // order to host byte stuffing order to get printed
        	    
        	    inet_ntoa(client.sin_addr));   // client IP address converted to network to ascii order

//==============================Check login and password===============================================        

        int client_type=-1;
        // strcpy(data,"Send login data and password in the form login:password");      //First message on client interface
        // data_len=strlen(data);
        // send(new, data, data_len, 0);

        while(client_type < 0) {
        req_len=recv(new, req, MAX_DATA, 0);  if(req_len==0) goto START;          //login password recieved
        req[req_len]='\0';
        printf("==================================================Request From Client============================\n%s\n=================================================================================================\n",req);
        if(strncmp(req,"GET menu",8)==0){
        data_len=con_len(req);
        //printf("length of data recieved%d\n",data,data_len);    
        strcpy(data,req+(req_len-data_len));
        int i=0,j=0;
        for(i=0;data[i]!=':' && i<data_len;i++)
        	login[i]=data[i];
        login[i]='\0';
        login_len=i;
        i++;
        for(;data[i]!='\0' && i<data_len;i++){
        	password[j]=data[i];
        	j++;
        }
        password[j-1]='\0';
        password_len=j;
        //printf("login_id: %s\n password: %s\n",login,password);
    }
        client_type= check_client_type(login,login_len,password,password_len);
        if(client_type<0){
        strcpy(data,"HTTP/1.1 403 Forbidden\nDate: ");
        strcat(data,ctime(&t));
        strcat(data,"Content-Length: 35\nContent-Type: text/plain; charset=utf-8\nConnection: Keep-Alive\n\nIncorrect login or password. Retry:");
        data_len=strlen(data);
        data_len=send(new, data, data_len, 0);
        printf("==================================================Response From Server===========================\n%s\n=================================================================================================\n",data);
        }
    }

    //printf("%d\n",client_type );


    if(client_type==1){
    	strcpy(data,"HTTP/1.1 200 OK\nDate: ");
        strcat(data,ctime(&t));
        strcat(data,"Content-Length: 63\nContent-Type: text/plain; charset=utf-8\nConnection: Keep-Alive\n\n1:Press:\n1. Account Balance\n2. Mini Statement\n#. Logout or exit");
        data_len=strlen(data); 
        data_len=send(new, data, data_len, 0);                  // Identify and send client its type
       // data_len=recv(new, data, MAX_DATA, 0);
}

	if(client_type==3){
		strcpy(data,"HTTP/1.1 200 OK\nDate: ");
        strcat(data,ctime(&t));
        strcat(data,"Content-Length: 97\nContent-Type: text/plain; charset=utf-8\nConnection: Keep-Alive\n\n3:Press:\n1. Account Balance\n2. Mini Statement with useridin the form 2:'userid'\n#. Logout or exit");
        data_len=strlen(data);
        data_len=send(new, data, data_len, 0);		
	}

    if(client_type==2){
        strcpy(data,"HTTP/1.1 200 OK\nDate: ");
        strcat(data,ctime(&t));
        strcat(data,"Content-Length: 73\nContent-Type: text/plain; charset=utf-8\nConnection: Keep-Alive\n\n2:Press:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");
        data_len=strlen(data); 
        data_len=send(new, data, data_len, 0);                  // Identify and send client its type
       // data_len=recv(new, data, MAX_DATA, 0);
}
printf("==================================================Response From Server===========================\n%s\n=================================================================================================\n",data);
      
        data_len=1;  // initialized to 1 to start off the following loop
		strcat(login,".txt");
        while(data_len)
        {	
        	
//======================================customer client=====================================================
            if(client_type==1 ){
        		
        	 FILE *fp=fopen(login,"r");
             req_len=recv(new, req, MAX_DATA, 0); //connected client socket 'new' sent_data which is 
             req[req_len]='\0';                                       //recieved by server in 'data' buffer of max size
             if(req_len==0) goto START;                                         //'MAX_DATA' and return number of bytes received. 
                                                    //'flags'==0
             
             printf("==================================================Request From Client============================\n%s\n=================================================================================================\n",req);
             int count=0;
             char data1[MAX_DATA]="";
             if(strncmp(req,"GET ACCOUNT_BALANCE",19)==0)  // if bytes are received by server from client, data_len>0 and following executes 
             {		while(fscanf(fp,"%s",data)!=EOF)
         			strcpy(data1,data);
                    strcpy(data,"The current account balance is: ");
                    strcat(data,data1);
                    strcat(data,"\n");
              }
              
              if(strncmp(req,"GET MINI_STATEMENT",18)==0){
                    data[0]='\0';
                    while(fscanf(fp,"%s",data)!=EOF){
                        strcat(data1,data);
                        if(count%3!=2)
                        strcat(data1,"   ");
                        else strcat(data1,"\n");
                        count++;
                    }
                    strcpy(data,"The current mini statement is:\n");
                    strcat(data,data1);         

              }  
                    //printf("data is:::;:::%s\n",data);
                 if(strncmp(req,"GET ACCOUNT_BALANCE",19)==0||strncmp(req,"GET MINI_STATEMENT",18)==0){
                    strcat(data,"\nPress:\n1. Account Balance\n2. Mini Statement\n#. Logout or exit");
                    data_len=strlen(data);
                    strcpy(res,"HTTP/1.1 200 OK\nDate: ");
                    strcat(res,ctime(&t));
                    strcat(res,"Content-Length: ");
                    sprintf(data1,"%d",data_len);
                    strcat(res,data1);
                    strcat(res,"\nContent-Type: text/plain\nConnection: Keep-Alive\n\n");
                    strcat(res,data);
                }
                 if(strncmp(req,"GET ACCOUNT_BALANCE",19)!=0&&strncmp(req,"GET MINI_STATEMENT",18)!=0&&strncmp(res,"GET CLOSE_REQUEST",17)!=0){
                  data_len=1;  
                 strcpy(res,"HTTP/1.1 400 Bad Request\nDate: ");
                 strcat(res,ctime(&t));
                 strcat(res,"Content-Length: 28\nContent-Type:text/plain\nConnection: Keep-Alive\n\nError 400: Bad Request.Retry");   
}
                   if(strncmp(req,"GET CLOSE_REQUEST",17)!=0){
                   res_len=send(new,res,strlen(res), 0); //'data' received by server will be immediately sent by
                                                 // server to connected client socket 'new'
                   
                   res[res_len]='\0';          // data is stored in data[i], i=0 to data_len-1.
                                                 // While printing data by printf, the end of string is marked 
                                                 // with null character '\0' so that printf not crash
                   printf("==================================================Response From Server===========================\n%s\n=================================================================================================\n",res);
                      }
                      else data_len=0;
        fclose(fp);
        }
//===================================administration client============================================
        if(client_type==2 ){
            int flag=0;
            char date[1024];
            char data1[MAX_DATA];
            req_len=recv(new, req, MAX_DATA, 0);    if(req_len==0) goto START;
            req[req_len]='\0';
            printf("==================================================Request From Client============================\n%s\n=================================================================================================\n",req);
            char data2[MAX_DATA]; 
            char data3[MAX_DATA];
                                                // TO remember the file name       
            if(strncmp(req,"POST CREDIT",11)==0){
                data_len=con_len(req);
                strcpy(data,req+(req_len-data_len));
                int i,j=0;
                for(i=0;data[i]!=':' && i<strlen(data);i++)
                data2[i]=data[i];
                data2[i]='\0';
                if(strlen(data)==i) flag=1;
                if(flag==0){
                i++;
                for(;i<strlen(data) && data[i]!='\0';i++,j++)
                data1[j]=data[i];
                data1[j]='\0';
                if(strlen(data1)>=8 || ase(data1)) flag=1;
                if(flag==0){
               // printf("%s       %s",data2,data1);
                strcat(data2,".txt");
                strcpy(data3,data2);
                FILE *fp=fopen(data2,"r");
                if(fp!=NULL){ 
                    while(fscanf(fp,"%s",data2)!=EOF)
                    strcpy(data,data2);
                    data[strlen(data)]='\0';
                    int balance=atoi(data);          
                    //printf("SENT MESSAGE BY SERVER: %s\n",data);
                    //data_len=recv(new, data, MAX_DATA, 0);
                    //data[data_len]='\0';
                    int credit=atoi(data1);
                    //printf("%d  %d\n",credit,balance);
                    balance=balance+credit;
                    //data=itoa(balance);
                    fclose(fp);
                    fp=fopen(data3,"a+");
                    sprintf(data2,"%d",balance);
                    strcpy(date,ctime(&t));
                    date[3]='_';
                    date[7]='_';
                    if(date[8]==' ') date[8]='_';
                    date[10]='_';
                    date[19]='_';
                    date[24]='\0';
                    fprintf(fp,"\n%s Credit_%s %s",date,data1,data2);
                    fclose(fp);
                    //send(new,data1,strlen(data1),0);
                    strcpy(data,"Succesfully done. Current balance is: ");
                    strcat(data,data2);
                    strcat(data,"\nPress:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");
                }
                else {flag=2;strcpy(data,"Incorrect customer id.\nPress:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");}
            }
        }
              if(flag==1) strcpy(data,"Incorrect format of message.\nPress:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");
                // data[data_len]='\0';
                // strcat(data,".txt");
                // strcpy(data2,data+2);
                
                // if(fp==NULL){send(new,"error",strlen("error"),0);}
                
            }


            if(strncmp(req,"POST DEBIT",10)==0){
                data_len=con_len(req);
                strcpy(data,req+(req_len-data_len));
                int i,j=0;
                for(i=0;data[i]!=':' && i<strlen(data);i++)
                data2[i]=data[i];
                data2[i]='\0';
                if(strlen(data)==i) flag=1;
                if(flag==0){
                i++;
                for(;i<strlen(data) && data[i]!='\0';i++,j++)
                data1[j]=data[i];
                data1[j]='\0';
                if(strlen(data1)>=8 || ase(data1)) flag=1;


                if(flag==0){
                strcat(data2,".txt");
                strcpy(data3,data2);
                //printf("%s       %s",data2,data1);
                FILE *fp=fopen(data2,"r");
                if(fp!=NULL){ 
                    while(fscanf(fp,"%s",data2)!=EOF)
                    strcpy(data,data2);
                    data[strlen(data)]='\0';
                    int balance=atoi(data);          
                    //printf("SENT MESSAGE BY SERVER: %s\n",data);
                    //data_len=recv(new, data, MAX_DATA, 0);
                    //data[data_len]='\0';
                    int debit=atoi(data1);
                    balance=balance-debit;
                  //  printf("%d    %d\n",balance,debit);
                    //data=itoa(balance);
                    if(balance<0) {flag=3; strcpy(data,"Error. Underflow of Balance.\nPress:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");}
                    else{
                    fclose(fp);
                    fp=fopen(data3,"a+");
                    sprintf(data2,"%d",balance);
                    strcpy(date,ctime(&t));
                    date[3]='_';
                    date[7]='_';
                    if(date[8]==' ') date[8]='_';
                    date[10]='_';
                    date[19]='_';
                    date[24]='\0';
                    fprintf(fp,"\n%s Debit_%s %s",date,data1,data2);
                    fclose(fp);
                    //send(new,data1,strlen(data1),0);
                    strcpy(data,"Succesfully done. Current balance is: ");
                    strcat(data,data2);
                    strcat(data,"\nPress:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");
                }
            }
                else {flag=2;strcpy(data,"Incorrect customer id.\nPress:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");}
            }}
              if(flag==1) strcpy(data,"Incorrect format of message. \nPress:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");
                // data[data_len]='\0';
                // strcat(data,".txt");
                // strcpy(data2,data+2);
                
                // if(fp==NULL){send(new,"error",strlen("error"),0);}
            }
            //printf("khsgfkbkjgbdkjbvgdbgf%s\n\n\n\n\n",data);
            if(strncmp(req,"POST DEBIT",10)!=0 && strncmp(req,"POST CREDIT",11)!=0 && strncmp(req,"GET CLOSE_REQUEST",17)!=0){flag=4; strcpy(data,"Error 400: Bad Request.\nPress:\n1.Credit to an account\n2.Debit from an account\n#. Logout or exit");}
            if(flag==0) strcpy(res,"HTTP/1.1 200 OK\nDate: ");
            if(flag==1 || flag==3) strcpy(res,"HTTP/1.1 400 Bad Request\nDate: ");
            if(flag==2) strcpy(res,"HTTP/1.1 404 Not Found\nDate: ");
         //   if(flag==3) strcpy(res,"HTTP/1.1 405 Method Not Allowed\nDate: ");
            strcat(res,ctime(&t));
            strcat(res,"Content-Length: ");
            sprintf(data2,"%ld",strlen(data));
            strcat(res,data2);
            strcat(res,"\nContent-Type: text/plain\nConnection: Keep-Alive\n\n");
            strcat(res,data);
            if(strncmp(req,"GET CLOSE_REQUEST",17)!=0) {res_len=send(new,res,strlen(res), 0);printf("==================================================Response From Server===========================\n%s\n=================================================================================================\n",res);}
            else data_len=0;
            res[res_len]='\0';
            
            
}

//===================================police client=====================================================
        
        if(client_type==3 ){
        	char data1[MAX_DATA];
            char data2[MAX_DATA];
        	req_len=recv(new, req, MAX_DATA, 0);   if(req_len==0) goto START;
        	req[req_len]='\0';
            printf("==================================================Request From Client============================\n%s\n=================================================================================================\n",req);
            data[0]='\0';

            if(strncmp(req,"GET ALL_BALANCES",16)==0){
                FILE *fp=fopen("login.txt","r");
                char user[MAX_DATA],pass[MAX_DATA],ct[MAX_DATA];
                while(fscanf(fp,"%s",user)!=EOF){
                    fscanf(fp,"%s %s",pass,ct);
                    if(ct[0]=='C'){
                        strcat(data,user);
                        strcat(data,"   ");
                        strcat(user,".txt");
                        FILE *fp1=fopen(user,"r");
                        while(fscanf(fp1,"%s",data1)!=EOF)
                        strcpy(data2,data1);
                        strcat(data,data2);
                        strcat(data,"\n");
                        fclose(fp1);
                        
                }
                }
                fclose(fp);
            }
                int flag=0;
            if(strncmp(req,"GET MINI_STATEMENT",18)==0){
                int count=0;
                data_len=con_len(req);
                //printf("%d\n\n\n",data_len);
                strcpy(data1,req+(req_len-data_len));
                strcat(data1,".txt");
                //printf("jadhsbvfjbdsfjsbdf%s\n",data1);
                FILE *fp=fopen(data1,"r");
                if(fp!=NULL){
                    flag=1;
                    data[0]='\0';data2[0]='\0';
                    while(fscanf(fp,"%s",data)!=EOF){
                        strcat(data2,data);
                        if(count%3!=2)
                        strcat(data2,"  ");
                        else strcat(data2,"\n");
                        count++;
                    }
                    strcpy(data,"The current mini statement is:\n");
                    strcat(data,data2);         
                    fclose(fp);
                }
                else flag=2;
            }    

            if((strncmp(req,"GET ALL_BALANCES",16)==0)||(strncmp(req,"GET MINI_STATEMENT",18)==0)&& flag==1){
                    strcat(data,"\nPress:\n1. Account Balance\n2. Mini Statement with useridin the form 2:'userid'\n#. Logout or exit");
                    data_len=strlen(data);
                    strcpy(res,"HTTP/1.1 200 OK\nDate: ");
                    strcat(res,ctime(&t));
                    strcat(res,"Content-Length: ");
                    sprintf(data1,"%d",data_len);
                    strcat(res,data1);
                    strcat(res,"\nContent-Type: text/plain\nConnection: Keep-Alive\n\n");
                    strcat(res,data);
                }    

            if(flag==2){
                strcpy(res,"HTTP/1.1 404 NOT FOUND\nDate: ");
                strcat(res,ctime(&t));
                data_len=1;
                strcat(res,"Content-Length: 132\nContent-Type: text/plain; charset=utf-8\nConnection: Keep-Alive\n\nThe customer account does not exist.\nPress:\n1. Account Balance\n2. Mini Statement with useridin the form 2:'userid'\n#. Logout or exit");
            }        
                
            if(strncmp(req,"GET ALL_BALANCES",16)!=0&&strncmp(req,"GET MINI_STATEMENT",18)!=0&&strncmp(res,"GET CLOSE_REQUEST",17)!=0){
                  data_len=1;  
                 strcpy(res,"HTTP/1.1 400 Bad Request\nDate: ");
                 strcat(res,ctime(&t));
                 strcat(res,"Content-Length: 28\nContent-Type:text/plain\nConnection: Keep-Alive\n\nError 400: Bad Request.\nPress:\n1. Account Balance\n2. Mini Statement with useridin the form 2:'userid'\n#. Logout or exit");   
}



                if(strncmp(req,"GET CLOSE_REQUEST",17)!=0)
                 {res_len=send(new,res,strlen(res), 0);            
                        //data[data_len]='\0';          
                        printf("==================================================Response From Server===========================\n%s\n=================================================================================================\n",res);
                  }     
                  else data_len=0;
          }

//=======================================================================================================

}
        printf("Client disconnected\n"); // when server not receive data from client		

		close(new);// that client connection is closed and command goes back to while(1) to connect
		 
         //printf("\nClose the Server?Enter Y or y:\n");
       //
         strcpy(ask,"ASDF");
         //printf("%s\n%d\n",ask,strlen(ask));
         while(strlen(ask)!=1)
         {
         printf("Close the Server?\n1.Yes:Enter Y or y\n2.No:Enter character other than Y or y\n");
         scanf("%s",ask);
         }
       //  printf("cd%cdc",ask);
         if(ask[0]=='Y' || ask[0]=='y')   break;       // to other client or this client again
         

       // if(pid==0) break;

		
	}
    printf("Shutting the server...\n");
    close(sock);   // if CTRL+C is pressed on server side, while(1) loop breaks and server socket 
                   // 'sock' is closed 

	//return 0;

}
