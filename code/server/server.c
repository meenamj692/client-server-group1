/* Group 1 FINAL PROJECT Server side Code*/

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
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
    exit(1);
}

void child(int newsd, int n);

int reader(char *line, char *argList[]);

int main(int argc, char *argv[])
{
	int sd, newsd, portno, clilength, status,ret,on;
	pid_t pid;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
		fprintf(stderr,"ERR:, no port provided\n");
		exit(1);
	}
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if (sd < 0) 
	error("ERR: opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
/* Enable address reuse */
on = 1;
ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sd, (struct sockaddr *) &serv_addr,
	    sizeof(serv_addr)) < 0) 
	    error("ERR: on binding");
	listen(sd,5);
	clilength = sizeof(cli_addr);
	while(1){
		newsd = accept(sd, (struct sockaddr *) &cli_addr, &clilength);
		if (newsd < 0) 
		  	error("ERR: on accept");
		bzero(buffer,256);

				printf("Congrats !! New Client has been connected\n");

		if((pid = fork()) == 0){
			child(newsd, n);
		}
		// waitpid(pid, &status, 0);
	}
		
	return 0; 
}

void child(int newsd, int n){
	int i;
	char message[256] = "Ctrl+D";

	while(1){
		char *executeString[20];
		char buffer[256] = {};
		// char *newbuffer[256] = {};
		// char newbuffer[256];
		n = read(newsd, &buffer,255);
		if (n < 0) error("ERR: reading from socket");
		if(strcmp(buffer, message) == 0){
			printf("Client asked to exit\n");
		    n = write(newsd, message, strlen(message));
			exit(1);
		}
		printf("Client: %s", buffer);
		
		int pipefd[2];
		pipe(pipefd);


		if (fork() == 0)
		{
		    close(pipefd[0]);    // close reading end in the child

		    dup2(pipefd[1], 1);  // send stdout to the pipe
		    dup2(pipefd[1], 2);  // send stderr to the pipe

		    close(pipefd[1]);    // this descriptor is no longer needed

			reader(buffer, executeString);
		    execvp(executeString[0], executeString);
    
		    printf("Unknown command\n");
		    exit(0);
		}
		else
		{
		    // parent
		    char sendbuffer[1024] = {};

		    close(pipefd[1]);  // close the write end of the pipe in the parent

		    sleep(1);
		    while (read(pipefd[0], sendbuffer, sizeof(sendbuffer)) != 0)
		    {
		    	n = write(newsd, sendbuffer, strlen(sendbuffer));
				if (n < 0) error("ERR: writing to socket");
		    	sleep(1);
		    }
		    close(pipefd[0]); 
		}


	}
}

int reader(char *line, char *argList[]){
  static int yes=0;
  int i=0, offset=0;
  char name[50];
  int found=0;

  while(yes & argList[i] != NULL)  // free previous memory
    free(argList[i++]);

  i=0;
  while(sscanf(line+offset, "%s", name)==1){      // copy next string from
    argList[i] = (char *) malloc(strlen(name)+1); // line to argList  
    strcpy(argList[i++], name);
    while(line[offset] == ' ')  // skip extra blanks   
      offset++; 
    offset += strlen(name);
  }

  if(!strcmp(argList[i-1], "&")){ // for background execution 
    argList[i-1] = NULL;
    found = 1;
  }
  else{
    if(argList[i-1][strlen(argList[i-1])-1] == '&'){
      argList[i-1][strlen(argList[i-1])-1]='\0';
      found = 1;
    } 
    argList[i] = NULL;
  }
  
  yes=1;
  return(found);
}
