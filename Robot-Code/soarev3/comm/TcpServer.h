
#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#define MAX_BUFFER_SIZE 65535

#include <pthread.h>

class TcpServer{
  public:
    TcpServer();
    
    virtual ~TcpServer();

    virtual bool start();

    bool sendPacket(const void* buffer, int buf_len);

    void setReceptionCallback(void (*f)(const void* buffer, int buf_len, void* user), void* user){
      this->callback = f;
      this->user = user;
    }

    bool isReady(){
      return initialized && connected;
    }

  private:
    static void* receiveThreadFunction(void* arg);

    bool receivePacket();

  private:
    pthread_t receiveThread;

    void (*callback)(const void *buffer, int buf_len, void* user);
    void* user;

    int socket_fd;
    int client_fd;

    bool connected;
    bool initialized;

    char buffer[MAX_BUFFER_SIZE];
};


#endif
