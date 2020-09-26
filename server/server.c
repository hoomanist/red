#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <limits.h>

#define ARG_MAX 64 * sizeof(char *)
#define DELIM " \t\n\r"


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
        printf("%s\n", arguments[0]);
        free(buffer);
        for(int i = 0; i < counter; i++) {
            free(arguments[i]);
        }
        close(client);
    }
}
