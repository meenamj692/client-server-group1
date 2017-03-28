/* Group 1 FINAL PROJECT Client Side Code */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    char message[256] = "Ctrl+D"; // Signal to quit
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    while(1){
	    printf("\n!!!-----Client-----!!!\n");
	    bzero(buffer,256);
	    // fgets(buffer,255,stdin);
	    if (fgets(buffer, 255, stdin) == NULL){
	    	n = write(sd, message, strlen(message));	    	
	    }else{
	    	n = write(sd, buffer,strlen(buffer));	    	
	    }
	    if (n < 0) 
	         error("ERR: writing to socket");
	    bzero(buffer,256);
	    n = read(sd,buffer,255);
	    if (n < 0) 
	         error("ERR: reading from socket");
	    printf("!!!-----Server-----!!!\n%s",buffer);
	    if(strcmp(buffer, message) == 0){
	    	printf("Exiting the system...\n");
	    	exit(1);
	    }
	}
    return 0;
}