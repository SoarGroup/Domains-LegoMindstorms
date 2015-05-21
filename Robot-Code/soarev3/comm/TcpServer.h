
#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#define MAX_BUFFER_SIZE 65535

#include <pthread.h>

class TcpServer{
  public:
    TcpServer();
    
    virtual ~TcpServer();

    virtual bool start();

    virtual void stop();

    bool isActive(){
      return initialized;
    }

    bool isConnected(){
      return initialized && connected;
    }

    bool sendPacket(const void* buffer, int buf_len);

    void setReceptionCallback(void (*f)(const void* buffer, int buf_len, void* user), void* user){
      this->callback = f;
      this->user = user;
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
