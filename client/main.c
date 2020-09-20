#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

struct result
{
    int port;
    char *host;
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

void red_prompt(struct result credential){
    printf("Address: %s\n", credential.host);
    printf("Port: %d\n", credential.port);
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