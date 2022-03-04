#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>

#define MAX 1024

int fileSize(char filename[] ){

    //Opening the file in read mode 
    FILE* fp = fopen(filename,"r") ;

    //Checking if file exists or not 
    if(fp==NULL){
        return 0;
    }

    //Moving the file pointer to the end 
    fseek(fp,0,SEEK_END) ;

    //Finding the position of the file pointer 
    int size = ftell(fp) ;

    //Closing the file
    fclose(fp) ;

    return size ;
}

char* getFilename(char str[]){
    char* ptr = strtok(str," ") ; //First word 
    ptr = strtok(NULL," ") ; //Second word
    return ptr ;

}

int main(int argc, char const *argv[])
{   
    int PORT ;
    printf("Enter port no. : ") ;
    scanf("%d",&PORT) ;

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
        perror("Bind failure ") ;
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

    char buffer[MAX] = {0} ;
    char filename[MAX] = {0} ;
    int valread = 0 ;
    valread = read(new_sock,buffer,sizeof(buffer)) ;
    printf("Client : %s \n",buffer) ;
    strcpy(filename , getFilename(buffer)) ;

    //Read n bytes from a file :
    int n = fileSize(filename) ;
    char message[100] ;
    FILE* fp = fopen(filename,"r") ;
    if(fp==NULL){
        printf("Server : File not found ! \n") ;
    }
    else{
        if(n==0){
            printf("Server : File empty ! \n") ;
        }
        else if(n>10){
            n= 10 ;
        }
        else{
            n-- ;
        }

        int i =0;
        char ch = fgetc(fp) ;
        while(i<n){
            message[i] = ch ;
            i++ ;

            ch = fgetc(fp) ;
        }
        message[i] = '\0' ;
        fclose(fp) ;    
    }

    printf("Server : %s\n",message) ;

    send(new_sock,message,n,0) ;
    
    printf("\nClosing connection \n") ;

	//Closing sockets
	close(new_sock) ;
	close(server_fd) ;
    
    return 0;
}
