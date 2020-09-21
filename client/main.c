#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <arpa/inet.h>
// maximum command size
#define ARG_MAX 64 * sizeof(char *)

// target server credentials(port and ip)
struct result
{
    int port;
    char *host;
};
// a socket credentials for loop 
struct target 
{ 
    int sock;
    struct sockaddr_in address;  
};
// parse a string to a `result' struct
struct result red_parse(char *input){
    char **tokens ;
    tokens = malloc(2 * sizeof(char *));
    char *token;
    if(tokens == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    token = strtok(input, ":");
    for(int i=0; token!=NULL; i++){
        tokens[i] = token;
        token = strtok(NULL, ":");
    }
    struct result output;
    output.host = tokens[0];
    output.port = atoi(tokens[1]);
    free(tokens);
    return output;
}
// connect to server
struct target red_connect(struct result credential){
    int sock;
    struct sockaddr_in serv_addr;
    if((sock = socket(AF_INET, SOCK_STREAM, 0) ) == 0){
        perror("socket()"); 
        exit(EXIT_FAILURE); 
    }
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(credential.port);
    // ip in form of string to binary representation
    if(inet_pton(AF_INET, credential.host, &serv_addr.sin_addr)<=0){ 
        perror("inet_pton()");
        exit(EXIT_FAILURE);
    }
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
        perror("connect()");
        exit(EXIT_FAILURE);
    }
    struct target output;
    output.address = serv_addr;
    output.sock = sock;
    return output; 
}

void red_prompt(struct result credential){
    struct target server;
    char *prompt = "$ "; // prompt shape

    for(;;){
        server = red_connect(credential);
        char *input= malloc(ARG_MAX);
        // get input using gnu readline
        input = readline(prompt);
        // new line character removal
        char *pos;
        if ((pos=strchr(input, '\n')) != NULL)
            *pos = ' ';
        // send the input
        send(server.sock , input , ARG_MAX , 0 ); 
        free(input);
    }
}

int main(int argc, char *argv[]){
    int opt;
    struct result credential;
    // cli arguments
    while ((opt = getopt(argc, argv, "c")) != -1)
    {
        switch (opt){
            case 'c':{ // c for connect
                credential = red_parse(argv[2]);
                red_prompt(credential);
                break;
            }
            default:{
                printf("Usage: red [-c] [ip:port]\n");
                break;
            }
        }
    }
}