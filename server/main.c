#include <stdio.h>
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <limits.h>


#define ARG_MAX 64 * sizeof(char *)

int main(int argc, char *argv[]){
    int sock, client, opt, port;
    struct sockaddr_in address; 
    int addrlen = sizeof(address);
    while ((opt = getopt(argc, argv, "p")) != -1) {
        switch (opt){
        case 'p':
            port = atoi(argv[2]);break;
        default:
            port = 5000;break;
        }
    }

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

        if (listen(sock, 3) < 0){ 
            perror("listen"); 
            exit(EXIT_FAILURE); 
        } 
        if ((client = accept(sock, (struct sockaddr *)&address,  
                        (socklen_t*)&addrlen))<0){ 
            perror("accept"); 
            exit(EXIT_FAILURE); 
        }
        read( client , buffer, ARG_MAX); 
        setbuf(stdout, NULL);
        printf("%s\n",buffer );
        free(buffer);
        close(client);
    }
}