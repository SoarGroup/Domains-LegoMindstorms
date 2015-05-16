
#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#define MAX_BUFFER_SIZE 65535

#include <pthread.h>

class TcpServer{
  public:
    TcpServer();
    
    ~TcpServer();

    bool start();

    bool sendPacket(const void* buffer, int buf_len);

    void setReceptionCallback(void (*f)(const void* buffer, int buf_len, void* user), void* user){
      this->user = user;
      this->callback = f;
    }

    bool isReady(){
      return initialized && connected;
    }

  private:
    pthread_t receiveThread;
    static void* receiveThreadFunction(void* arg);
    bool receivePacket();

    void (*callback)(const void *buffer, int buf_len, void *user);
    void* user;

    int socket_fd;
    int client_fd;

    bool connected;
    bool initialized;

    char buffer[MAX_BUFFER_SIZE];
};


#endif
