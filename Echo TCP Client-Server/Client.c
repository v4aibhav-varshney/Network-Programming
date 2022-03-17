#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 4445

void readline(char * str,int size,char delim){
    int i = 0 ;
    char ch = getchar() ;
    while(ch!=delim){
        if(i==size-1){ //To save space for null character 
            break ;
        }
        str[i] = ch ;
        ch = getchar() ;
        i++ ;
    }
    str[i] = '\0' ; //Null character 
    return ;
}

void chat(int sock_fd){
    char buffer[MAX] ;

    while(1) {
        bzero(buffer,sizeof(buffer)) ;
        printf("Enter string : ") ;
        readline(buffer,MAX,'\n') ;

        printf("Client : %s \n",buffer) ;
        send(sock_fd,buffer,sizeof(buffer),0) ;

        if(strcmp(buffer,"Exit")==0){
            break ;
        }
    }
}


int main(int argc, char const *argv[])
{
	int sock_fd, new_sock;
	struct sockaddr_in address; //Server address

    //Network socket creation 
	if((sock_fd = socket(AF_INET,SOCK_STREAM,0))<0) {
        perror("Socket failure") ;
        exit(EXIT_FAILURE) ;
    }

    //Server address specifications 
	address.sin_family = AF_INET ;
    address.sin_addr.s_addr = inet_addr("127.0.0.1") ;
	address.sin_port = htons(PORT) ;

    //Connecting to the server 
    if(connect(sock_fd , (struct sockaddr *)&address, sizeof(address))<0){
        perror("Connection failure") ;
        exit(EXIT_FAILURE) ;
    }

    printf("Connected \n\n") ;

    //Communication with server 
    chat(sock_fd) ;

    printf("\nClosing connection \n") ;

	//Closing sockets
	close(sock_fd) ;
    
    return 0;
}
