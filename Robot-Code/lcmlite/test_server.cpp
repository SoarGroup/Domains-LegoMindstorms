#include <iostream>
#include <string>
#include <unistd.h>

#include "TcpServer.h"

using namespace std;

void getPacket(const void* buf, int buf_len, void* user){
  cout << "Got Packet: " << (const char*)buf << endl;
}

int main(int argc, char** argv){
  TcpServer server;
  server.setReceptionCallback(&getPacket, 0);
  server.start();

  while(true){
    string message = "Hello from the Robot";
    if(server.isReady()){
      server.sendPacket((const void*)message.c_str(), message.size()+1);
    }
    usleep(2000000);
  }



  return 1;
}
