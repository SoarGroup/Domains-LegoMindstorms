#include "TcpClient.h"

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

using namespace std;


TcpClient::TcpClient(string server_ip)
  :server_ip(server_ip), connected(false), callback(0), user(0){
}

TcpClient::~TcpClient(){
  if(connected){
    close(socket_fd);
  }
}

bool TcpClient::openConnection(){
  if (connected){
    printf("TcpClient Error: Trying to connect when its already connected\n");
    return false;
  }

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

  int flag = 1;
  if (setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag)) < 0){
    perror("TcpServer Error: setsockopt TCP_NODELAY");
  }

  connected = true;
  printf("TcpClient: socket is connected\n");

	pthread_create(&receiveThread, 0, &receiveThreadFunction, this);
  return true;
}

void TcpClient::closeConnection(){
  if(connected){
    close(socket_fd);
    connected = false;
  }
  pthread_join(receiveThread, NULL);
}

#include <ctime>

#include <sys/time.h>

void* TcpClient::receiveThreadFunction(void* arg){
  TcpClient* server = (TcpClient*)arg;

  int num_packets = 0;
  long last_time = (long)time(0);

  while(server->isConnected()){
    if(!server->receivePacket()){
      return 0;
    }
    num_packets++;
    long now = (long)time(0);
    if (now != last_time){
      //printf("FPS: %d\n", num_packets);
      num_packets = 0;
    }
    last_time = now;
  }

  return 0;
}

bool TcpClient::receivePacket(){
  //printf("TcpClient: listening for packet\n");
  ssize_t len = read(socket_fd, (void*)buffer, MAX_BUFFER_SIZE-1);
  if (len < 0){
    perror("TcpClient Error: read");
    connected = false;
    return false;
  }
  if (len == 0){
    printf("TcpClient: connection closed\n");
    connected = false;
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
    connected = false;
    return false;
  }
  //printf("TcpClient: successfully sent packet\n");
  return true;
}




