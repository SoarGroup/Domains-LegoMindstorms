/*
 * comm/TcpServer.h
 *
 *  Created on: May 15, 2015
 *      Author: Aaron
 *
 *  TcpServer is a class that implements a simple server using TCP/IP protocols
 *    It uses unix sockets and so is only for linux/mac
 *  It supports a single connection at a time on port 7667
 *    And a maximum packet size of 65535 bytes when receiving
 */

#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#define MAX_BUFFER_SIZE 65535
#define DEFAULT_PORT 7667

#include <pthread.h>

class TcpServer{
  public:
    TcpServer();
    
    virtual ~TcpServer();

    // Causes the server to open the sockets and start listening for a connection
    // Also spawns the receiveThread to listen for incoming messages and pass them
    //  on using the receptionCallback
    virtual bool start();

    // Causes the server to close the current connection and close the sockets
    // Will also stop the receiveThread 
    virtual void stop();

    // Is true when there is an active connection with a client
    bool isConnected(){
      return initialized && connected;
    }

    // Sends the given information to the client (if connected)
    // Returns true if the info was successfully sent
    bool sendPacket(const void* buffer, int buf_len);

    // Sets the callback to be called when a message is received
    // buf_len is the length in bytes, and user is the same pointer given in this call
    void setReceptionCallback(void (*f)(const void* buffer, int buf_len, void* user), void* user){
      this->callback = f;
      this->user = user;
    }

  private:
    // The function called when a new thread is created
    //   Will listen for a new client connection, 
    //   then call receivePacket while the connection is open
    static void* receiveThreadFunction(void* arg);

    //   Will listen for new messages from the client and call the callback function
    bool receivePacket();

  private:
    pthread_t receiveThread;

    void (*callback)(const void *buffer, int buf_len, void* user);
    void* user;

    int socket_fd;
    int client_fd;

    bool connected;   // True if there is a client actively connected
    bool initialized; // True if the server socket is created

    char buffer[MAX_BUFFER_SIZE];
};


#endif
