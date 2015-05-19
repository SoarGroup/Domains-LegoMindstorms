

#include "TcpServer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

TcpServer::TcpServer()
  :initialized(false), connected(false), callback(0){
}

TcpServer::~TcpServer(){
  if(connected){
    close(client_fd);
  }
  if(initialized){
    close(socket_fd);
  }
}

bool TcpServer::start(){
  printf("TcpServer: creating socket\n");
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0){
    perror("TcpServer Error: socket()");
    initialized = false;
    return false;
  }

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(7667);

  if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    perror("TcpServer Error: bind");
    close(socket_fd);
    return false;
  }

  int flag = 1;
  if (setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag)) < 0){
    perror("TcpServer Error: setsockopt TCP_NODELAY");
    close(socket_fd);
    return false;
  }


  initialized = true;
  listen(socket_fd, 5);

  printf("TcpServer: socket is initialized\n");

	pthread_create(&receiveThread, 0, &receiveThreadFunction, this);
  return true;
}

void* TcpServer::receiveThreadFunction(void* arg){
  TcpServer* server = (TcpServer*)arg;
  if (!server->initialized){
    return 0;
  }
  
  while(true){
    printf("TcpServer: listening for new connection\n");

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    server->client_fd = accept(server->socket_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if (server->client_fd < 0){
      perror("TcpServer Error: accept");
      return 0;
    }
    printf("TcpServer: connection accepted\n");

    server->connected = true;
    while(server->receivePacket()){
      // Nothing
    }

    server->connected = false;
    printf("TcpServer: connection closed\n");
  }

  return 0;
}

bool TcpServer::receivePacket(){
  //printf("TcpServer: listening for packet\n");
  ssize_t len = read(client_fd, (void*)buffer, MAX_BUFFER_SIZE-1);
  if (len < 0){
    perror("TcpServer Error: read");
    return false;
  }
  if (len == 0){
    return false;
  }
  //printf("TcpServer: received packet of size %d\n", (int)len);
  if (callback != 0){
    callback(buffer, len, user);
  }
  return true;
}
  
bool TcpServer::sendPacket(const void* buffer, int buf_len){
  //printf("TcpServer: sending packet of length %d\n", buf_len);
  if (!initialized || !connected){
    printf("TcpServer Error: Trying to send on bad connection\n");
    return false;
  }

  if (write(client_fd, buffer, buf_len) < 0){
    perror("TcpServer Error: write");
    return false;
  }
  //printf("TcpServer: successfully sent packet\n");
  return true;
}
