/*
 * comm/TcpClient.h
 *
 *  Created on: May 15, 2015
 *      Author: Aaron
 *
 *  TcpClient is a class that implements a simple client that
 *    connects to a server using TCP/IP protocols
 *    It uses unix sockets and so is only for linux/mac
 *  It supports a single connection at a time on port 7667
 *    And a maximum packet size of 65535 bytes when receiving
 */

#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include <string>
#include <pthread.h>

#define MAX_BUFFER_SIZE 65535
#define DEFAULT_PORT 7667

class TcpClient{
  public:
    // server_ip is the ip address of the server to connect to
    // (i.e. "192.168.1.5")
    TcpClient(std::string server_ip);
    
    virtual ~TcpClient();

    // Try to open a connection to the server
    //   Will spawn the receiveThread to listen for packets
    //   Returns true if successful
    virtual bool openConnection();

    // Closes the connection to the server
    //   And will kill the receiveThread
    virtual void closeConnection();

    // Sends the given information to the server (if connected)
    // Returns true if the info was successfully sent
    bool sendPacket(const void* buffer, int buf_len);

    // Sets the callback to be called when a message is received
    // buf_len is the length in bytes, and user is the same pointer given in this call
    void setReceptionCallback(void (*f)(const void* buffer, int buf_len, void* user), void* user){
      this->callback = f;
      this->user = user;
    }

    // Returns true if the client is actively connected to the server
    bool isConnected(){
      return connected;
    }

  private:
    // Function called when the receiveThread is created
    //   Will continuosly call receivePacket while the connection is open
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
