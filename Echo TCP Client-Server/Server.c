#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define MAX 1024
#define PORT 4445

void chat(int sock_fd){
    char buffer[MAX] ;
    int valread = 0 ;

    while(1) {
        bzero(buffer,sizeof(buffer)) ;
        read(sock_fd,buffer,sizeof(buffer)) ;
        printf("Client : %s \n",buffer) ;

        if(strcmp(buffer,"Exit")==0){
            break ;
        }
    }

}


int main(int argc, char const *argv[])
{
	int server_fd, new_sock;
	struct sockaddr_in address; //Sever address 

    //Network socket creation 
	if((server_fd = socket(AF_INET,SOCK_STREAM,0))<0) {
        perror("Socket failure") ;
        exit(EXIT_FAILURE) ;
    }

    //Server address specifications 
	address.sin_family = AF_INET ;
	address.sin_addr.s_addr = INADDR_ANY ;
	address.sin_port = htons(PORT) ;

    //Socket binding to the port 
	if(bind(server_fd,(struct sockaddr *) &address ,sizeof(address))<0){
        perror("Bind failure !") ;
        exit(EXIT_FAILURE) ;
    }

    //Listening for connections 
	if(listen(server_fd,5)<0) {
        perror("Listen failure") ;
        exit(EXIT_FAILURE) ;
    }

    printf("Listening \n") ;

    //Accepting the connection 
    int addrlen = sizeof(address) ;
	if((new_sock = accept(server_fd,(struct sockaddr *) &address,(socklen_t* )&addrlen ))<0) {
        perror("Accept failure") ;
        exit(EXIT_FAILURE) ;
    } 

    printf("Accepted \n\n") ;

    //Communication with client 
    chat(new_sock) ;

	//Closing sockets
    printf("\nClosing connection \n") ;
	close(new_sock) ;
	close(server_fd) ;
    
    return 0;
}
