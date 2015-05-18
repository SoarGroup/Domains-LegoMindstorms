#include "TcpClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

using namespace std;


TcpClient::TcpClient(string server_ip)
  :server_ip(server_ip), connected(false), callback(0), user(0){
}

TcpClient::~TcpClient(){
  if(connected){
    close(socket_fd);
  }
}

bool TcpClient::start(){
  printf("TcpClient: creating socket\n");
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0){
    perror("TcpClient Error: socket()");
    return false;
  }

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
	server_addr.sin_port = htons(7667);

  printf("TcpClient: connecting\n");
  if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    perror("TcpClient Error: connect");
    close(socket_fd);
    return false;
  }

  connected = true;
  printf("TcpClient: socket is connected\n");

	pthread_create(&receiveThread, 0, &receiveThreadFunction, this);
  return true;
}

#include <ctime>

#include <sys/time.h>

void* TcpClient::receiveThreadFunction(void* arg){
  TcpClient* server = (TcpClient*)arg;
  if (!server->connected){
    return 0;
  }

  int num_packets = 0;
  long last_time = (long)time(0);

  while(server->receivePacket()){
    long now = (long)time(0);
    if (now != last_time){
      //printf("FPS: %d\n", num_packets);
      num_packets = 0;
    }
    last_time = now;
    num_packets++;

    // Do nothing
  }

  server->connected = false;
  return 0;
}

bool TcpClient::receivePacket(){
  //printf("TcpClient: listening for packet\n");
  ssize_t len = read(socket_fd, (void*)buffer, MAX_BUFFER_SIZE-1);
  if (len < 0){
    perror("TcpClient Error: read");
    return false;
  }
  if (len == 0){
    printf("TcpClient: connection closed\n");
    return false;
  }
  //printf("TcpClient: received packet of size %d\n", (int)len);
  if (callback != 0){
    callback(buffer, len, user);
  }
  return true;
}
  
bool TcpClient::sendPacket(const void* buffer, int buf_len){
  //printf("TcpClient: sending packet of length %d\n", (int)buf_len);
  if (!connected){
    perror("TcpClient Error: Trying to send on bad connection");
    return false;
  }

  if (write(socket_fd, buffer, buf_len) < 0){
    perror("TcpClient Error: write");
    return false;
  }
  //printf("TcpClient: successfully sent packet\n");
  return true;
}




