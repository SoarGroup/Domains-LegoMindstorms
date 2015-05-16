#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void error(const char* msg){
  perror(msg);
  exit(0);
}

int main(int argc, char** argv){
  int client_fd;

  printf("socket\n");
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0){
    error("ERROR: socket()");
  }

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(7667);

  printf("connect\n");
  if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    error("ERROR: connect");
  }

  printf("ready\n");

  const int BUF_LEN = 256;
  char buffer[BUF_LEN];

  while(true){
    memset(buffer, 0, BUF_LEN);
    fgets(buffer, BUF_LEN-1, stdin);
    printf("write\n");
    if (write(client_fd, buffer, strlen(buffer)+1) < 0){
      error("ERROR: write");
    }
  }

  close(client_fd);

  return 1;
}
