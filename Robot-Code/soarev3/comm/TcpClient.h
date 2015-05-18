#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>
#include <pthread.h>

#define MAX_BUFFER_SIZE 65535

class TcpClient{
  public:
    TcpClient(std::string server_ip);
    
    virtual ~TcpClient();

    bool start();

    bool sendPacket(const void* buffer, int buf_len);

    void setReceptionCallback(void (*f)(const void* buffer, int buf_len, void* user), void* user){
      this->callback = f;
      this->user = user;
    }

    bool isConnected(){
      return connected;
    }

  private:
    static void* receiveThreadFunction(void* arg);

    bool receivePacket();

  private:
    char buffer[MAX_BUFFER_SIZE];
    
    std::string server_ip;
    int socket_fd;
    bool connected;

    void (*callback)(const void *buffer, int buf_len, void* user);
    void* user;

    pthread_t receiveThread;
};



#endif
