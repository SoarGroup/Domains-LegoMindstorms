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
  int server_fd;

  printf("socket\n");
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0){
    error("ERROR: socket()");
  }

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(7667);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    error("ERROR: bind");
  }

  listen(server_fd, 5);

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
  if (client_fd < 0){
    error("ERROR: accept");
  }

  printf("ready\n");

  const int BUF_LEN = 256;
  char buffer[BUF_LEN];

  while(true){
    if (read(client_fd, buffer, BUF_LEN-1) < 0){
      error("ERROR: read");
    }
    printf("%s\n", buffer);
  }

  close(client_fd);
  close(server_fd);

  return 1;
}
