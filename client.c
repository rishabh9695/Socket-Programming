/*
A simple echo client:

./server server_ip server_port_no   //ip and port_no to which server is listening to

*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <error.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>

//macros

#define ERROR -1
#define BUFFER 102400

int con_len(char req[]){
    int count=0 ,i;
    for(i=0;req[i]!='\0' ;i++){
        //printf("char[]==%c\n",req[i]);
        if(req[i]=='\n') count++;
        if(count==2) break;
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





void main(int argc, char **argv)
{
	
	struct sockaddr_in remote_server;
	int sock;    // socket for server
		
	char input[BUFFER];
	char output[BUFFER];
	char req[BUFFER];
	char res[BUFFER];
	int len;int req_len,res_len;
 
	int sockaddr_len;	
	sockaddr_len= sizeof(struct sockaddr_in);
	

    // checking for server socket, if not successful gives error  
	if((sock = socket(AF_INET, SOCK_STREAM,0))== ERROR) 
	{
		perror("server socket:");
		exit(-1);
	}

	remote_server.sin_family=AF_INET;
	remote_server.sin_port=htons(atoi(argv[2]));        // argv[2]== port_no
	remote_server.sin_addr.s_addr= inet_addr(argv[1]);  // argv[1]== ip
	bzero(&remote_server.sin_zero,8);
	
	// client checking connection with above socket 'sock' with server sockaddr
	if( (connect(sock,(struct sockaddr *) &remote_server, sockaddr_len))== ERROR)
	{
		perror("connect");
		exit(-1);
	}
	
	output[0]='\0';
	printf("Enter user_id and password in format login:password\n");
  	strcpy(res,"HTTP/1.1 403 Forbidden");
	while(strncmp(res,"HTTP/1.1 403 Forbidden",22)==0){
		
		printf("%s\n",output);	
		fgets(input, BUFFER, stdin);                
		strcpy(req,"GET menu HTTP/1.1\nHost:");
		strcat(req,argv[1]);
		strcat(req,"\nContent-Type: text/plain; charset=utf-8\nContent-Length: ");
		sprintf(output,"%ld",strlen(input));
		strcat(req,output);
		strcat(req,"\nAccept-language: en-us\nAccept-Encoding:\nConnection: Keep-Alive\n\n");
		strcat(req,input);
		len=send(sock, req, strlen(req),0 );         // sending login and password
		//printf("\n------------------------------------Client request Sent------------------------\n%s\n-------------------------------------------------------------------------------\n",req);
		res_len= recv(sock, res, BUFFER, 0);         	// understanding type of client 
		res[res_len] = '\0';
		len=con_len(res);
		strcpy(output,res+(res_len-len));
		
		////printf("stsedgve%d\n",strlen(output));
	}

		printf("%s\n",output+2);
		char imp=output[0];

		////printf("abcd%sabcd\n",output);
		
    while(1){ 	

    	////printf("input:%s\nstrlen(input):%d\n",input,strlen(input));
		input[0]='8';//strcpy(output," ");
		
		////printf("output:%s\nstrlen(output):%d\n",output,strlen(output));
		////printf("input:%s\nstrlen(input):%d\n",input,strlen(input));
//=========================================== BANK CUSTOMER================================================		
		if(imp=='1'){ 
		while((input[0]!='1' && input[0] !='2' && input[0]!='#' )|| (strlen(input)!=2)){		
		//printf("%s\n",output+2)	;
		fgets(input, BUFFER, stdin);                //connected client takes input from user through 
        //  scanf("%s",input);                                          //standard input 'stdin' and stores in 'input' buffer
        }                                            //of max size 'BUFFER'
		
		strcpy(req,"GET ");
		if(input[0]=='1')
		strcat(req,"ACCOUNT_BALANCE ");
		if(input[0]=='2')
		strcat(req,"MINI_STATEMENT ");
		if(input[0]=='#')
		strcat(req,"CLOSE_REQUEST ");
		strcat(req,"HTTP/1.1\nHost: ");
		strcat(req,argv[1]);
		strcat(req,"\nAccept-language: en-us\nAccept-Encoding: \nConnection: ");
		if(input[0]=='#')
		strcat(req,"Close\n");
		else strcat(req,"Keep-Alive\n");
		req_len=send(sock, req, strlen(req),0 );        //'input' received from user in client side will be 
		                                            //immediately sent to server socket 'sock' of length 
		                                            //'strlen(input)'. 'flags'==0
		//printf("\n------------------------------------Client request Sent------------------------\n%s\n-------------------------------------------------------------------------------\n",req);
		
		if(input[0]=='1' || input[0]=='2'){
		res_len= recv(sock, res, BUFFER, 0);         		
		res[res_len] = '\0';
		len=con_len(res);
		strcpy(output,res+(res_len-len));                        
		printf("%s\n",output);
		}	

		if(input[0]=='#'&& strlen(input)==2){
			//printf("input_in_#_case:%s\n",input);
			break;
		}


	}
//==========================================Administrator==============================================
	if(imp=='2'){
		char output1[BUFFER];
		while((input[0]!='1' && input[0] !='2' && input[0]!='#') || (strlen(input)!=2)){	
		//printf("%s\n",output+2)	;
		//scanf("%s",input);
		fgets(input, BUFFER, stdin);                //connected client takes input from user through 
        //input[strlen(input)]='\0';
        //                                          //standard input 'stdin' and stores in 'input' buffer
        }
        //printf("%s\n",input);
        if(input[0]!='#'){
        strcpy(req,"POST ");
        if(input[0]=='1')
        	strcat(req,"CREDIT ");
        else strcat(req,"DEBIT ");
}
		else strcpy(req,"GET CLOSE_REQUEST ");
		strcat(req,"HTTP/1.1\nHost: ");
		strcat(req,argv[1]);
		if(input[0]!='#'){
		printf("Type the account name and amount in format user_id:amount\n");
		scanf("%s",output1);	
		strcat(req,"\nContent-Type: text/plain\nContent-Length: ");
		sprintf(res,"%ld",strlen(output1));
		strcat(req,res);
	}
		strcat(req,"\nAccept-language: en-us\nAccept-Encoding: \nConnection: ");
		if(input[0]=='#'){
			strcat(req,"CLOSE\n");
		}

		else{
			strcat(req,"Keep-Alive\n\n");
			strcat(req,output1);
		}
		
		req_len=send(sock, req, strlen(req),0 ); 
		req[req_len]='\0';
		//printf("%s\n",req);
		if(input[0]=='1' || input[0]=='2'){
		res_len= recv(sock, res, BUFFER, 0);         		
		res[res_len] = '\0';
		len=con_len(res);
		strcpy(output,res+(res_len-len));                        
		printf("%s\n",output);
		}	

		

         if(input[0]=='#'){
		 	break;
		 } 
		   

	}	

//==========================================POLICE========================================================
 	if(imp=='3'){
 		while((input[0]!='1' && input[0] !='2' && input[0]!='#') ||(input[0]=='2' && strlen(input)<3)){	
			scanf("%s",input);                                         //standard input 'stdin' and stores in 'input' buffer
        }                                            //of max size 'BUFFER'
        
        strcpy(req,"GET ");
        if(input[0]=='1')
        strcat(req,"ALL_BALANCES ");
    	if(input[0]=='2')
    	strcat(req,"MINI_STATEMENT ");
    	if(input[0]=='#')
    	strcat(req,"CLOSE_REQUEST ");
    	strcat(req,"HTTP/1.1\nHost: ");
    	strcat(req,argv[1]);
    	
    	if(input[0]!='2'){
    		strcat(req,"\nAccept-language: en-us\nAccept-Encoding: \nConnection: ");
    		if(input[0]=='#')
    		strcat(req,"Close\n");
    		else strcat(req,"Keep-Alive\n");
    	}

    	if(input[0]=='2'){
    		strcat(req,"\nContent-Type: text/plain\nContent-Length: ");
    		sprintf(output,"%ld",strlen(input)-2);
    		strcat(req,output);
    		strcat(req,"\nAccept-language: en-us\nAccept-Encoding: \nConnection: Keep-Alive\n\n");
    		strcat(req,input+2);
    	}
        req_len=send(sock,req, strlen(req),0 );
        req[req_len]='\0';
        // printf("jhdgfdjgfdhjsgf%s\n",req);
        if(input[0]=='1' || input[0]=='2'){
		res_len= recv(sock, res, BUFFER, 0);         		
		res[res_len] = '\0';
		len=con_len(res);
		//printf("jhdgfdjgfdhjsgf%d\n",len);
		strcpy(output,res+(res_len-len));                        
		printf("%s\n",output);
		}	

		// if(input[0]=='#'&& strlen(input)==2){
		// 	//printf("input_in_#_case:%s\n",input);
		// 	break;
		// }

         if(input[0]=='#'){
		 	break;
		 }
 	}
//====================================================================================================



if(input[0]=='#' && strlen(input)==2) break;


}

    close(sock);   

	// return 0;

}
