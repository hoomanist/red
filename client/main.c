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

struct result
{
    int port;
    char *host;
};

struct target 
{ 
    int sock;
    struct sockaddr_in address;  
};

struct result red_parse(char *input){
    int i = 0;
    char **tokens ;
    tokens = malloc(2 * sizeof(char *));
    char *token;
    if(tokens == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    token = strtok(input, ":");
    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        token = strtok(NULL, ":");
    }
    struct result output;
    output.host = tokens[0];
    output.port = atoi(tokens[1]);
    free(tokens);
    return output;
}

struct target red_connect(struct result credential){
    int sock;
    struct sockaddr_in serv_addr;
    if((sock = socket(AF_INET, SOCK_STREAM, 0) ) == 0){
        perror("socket()"); 
        exit(EXIT_FAILURE); 
    }
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(credential.port);
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
    char *prompt = "$ ";

    server = red_connect(credential);
    for(;;){
        char *input= malloc(64 * sizeof(char *));
        input = readline(prompt);
        send(server.sock , input , strlen(input) , 0 ); 
        free(input);
    }
    
}

int main(int argc, char *argv[]){
    int opt;
    struct result credential;

    while ((opt = getopt(argc, argv, "c")) != -1)
    {
        switch (opt){
        case 'c':
            credential = red_parse(argv[2]);red_prompt(credential);break;
        default:
            printf("Usage: red [-c] [ip:port]\n");
            break;
        }
    }
    

}