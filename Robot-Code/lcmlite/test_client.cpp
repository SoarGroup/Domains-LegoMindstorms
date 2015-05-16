#include <iostream>

#include "TcpClient.h"

using namespace std;

void getPacket(const void* buf, int buf_len, void* user){
  cout << "Got Message: " << (const char*)buf << endl;
}

int main(int argc, char** argv){
  if (argc < 2){
    cout << "Arg 1: IP Address of server" << endl;
    return 0;
  }

  TcpClient client(argv[1]);
  client.setReceptionCallback(&getPacket, 0);
  client.start();

  cout << "Enter words to send" << endl;
  while(true){
    string word;
    cin >> word;
    client.sendPacket((const void*)word.c_str(), word.size() + 1);
  }

  return 1;
}
