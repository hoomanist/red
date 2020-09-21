#include <limits.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int sock, client, opt, port;
  char buffer[CHAR_MAX];
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  while ((opt = getopt(argc, argv, "p")) != -1) {
    switch (opt) {
    case 'p':
      port = atoi(argv[2]);
      break;
    default:
      port = 5000;
      break;
    }
  }

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind()");
    exit(EXIT_FAILURE);
  }
  if (listen(sock, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  for (;;) {

    if ((client = accept(sock, (struct sockaddr *)&address,
                         (socklen_t *)&addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
    read(client, buffer, CHAR_MAX);
    setbuf(stdout, NULL);
    printf("%s", buffer);
  }
}