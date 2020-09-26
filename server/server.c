#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <limits.h>
#include <sys/wait.h>
#include <sys/types.h>

#define ARG_MAX 64 * sizeof(char *)
#define DELIM " \t\n\r"

struct spawn {
    char text[4096];
    int status;
};

struct spawn red_spawn(char **argv) {
  int status;
  int link[2];
  char text[4096];
  if (pipe(link)==-1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  switch(fork()) {
    case -1: perror("fork"); exit(EXIT_FAILURE);
    case 0:
        dup2 (link[1], STDOUT_FILENO);
        close(link[0]);
        close(link[1]);
        execvp(argv[0], argv);
        perror("execvp");
        _exit(EXIT_FAILURE);
    default:

      close(link[1]);
      int nbytes = read(link[0], text, sizeof(text));
      wait(&status); /*TODO: set $? to the status*/
  }
  struct spawn output;
  output.status = status;
  strncpy(output.text, text, sizeof(text));
  return output;
}

char **red_convertion(char *line, int *counter){
    int i;
    char **tokens = malloc(ARG_MAX); 
    char *token = strtok(line, DELIM);
    for(; token != NULL ;i++){
        tokens[i] = token;
        token = strtok(NULL, DELIM);
    }
    *counter = i;
    return tokens;
}

int main(int argc, char *argv[]){
    int sock, client, opt, port;
    struct sockaddr_in address; 
    int addrlen = sizeof(address);
    // cli arguments
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt){
        case 'p': // p for port
            port = atoi(argv[2]);break;
        default:
            port = 5000;break;
        }
    }
    // ipv4 TCP socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0){ 
        perror("socket()"); 
        exit(EXIT_FAILURE); 
    }

    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( port ); 

    if (bind(sock, (struct sockaddr *)&address,  
                                 sizeof(address))<0){ 
        perror("bind()"); 
        exit(EXIT_FAILURE); 
    }
    for(;;){
        char *buffer = malloc(ARG_MAX);
        int counter;
        if (listen(sock, 3) < 0){ 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        } 
        if ((client = accept(sock, (struct sockaddr *)&address,  
                        (socklen_t*)&addrlen))<0){ 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        int len = read( client , buffer, ARG_MAX); 
        setbuf(stdout, NULL);
        buffer[len+1] = '\0'; // termination code
        char **arguments = red_convertion(buffer, &counter);
        struct spawn answer;
        answer = red_spawn(arguments);
        send(client , answer.text , strlen(answer.text) , 0 ); 
        free(arguments);
        free(buffer);
        close(client);
    }
}
