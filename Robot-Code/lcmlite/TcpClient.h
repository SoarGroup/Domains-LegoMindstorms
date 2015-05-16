#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>
#include <pthread.h>

#define MAX_BUFFER_SIZE 65535

class TcpClient{
  public:
    TcpClient(std::string server_ip);
    
    ~TcpClient();

    bool start();

    bool sendPacket(const void* buffer, int buf_len);

    void setReceptionCallback(void (*f)(const void* buffer, int buf_len, void* user), void* user){
      this->user = user;
      this->callback = f;
    }

    bool isReady(){
      return connected;
    }

  private:
    pthread_t receiveThread;
    static void* receiveThreadFunction(void* arg);
    bool receivePacket();

    char buffer[MAX_BUFFER_SIZE];
    
    std::string server_ip;
    int socket_fd;
    bool connected;

    void (*callback)(const void *buffer, int buf_len, void *user);
    void* user;
};



#endif
