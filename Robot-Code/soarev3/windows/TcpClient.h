#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#define __USE_W32_SOCKETS
#include <winsock2.h>

#include <string>

//#include <Windows.h>

#define MAX_BUFFER_SIZE 65535

class TcpClient {
public:
    TcpClient(std::string server_ip);

    virtual ~TcpClient();

	virtual bool openConnection();

	virtual void closeConnection();

    bool sendPacket(const void* buffer, int buf_len);

    void setReceptionCallback(void (*f)(const void* buffer, int buf_len, void* user), void* user) {
        this->callback = f;
        this->user = user;
    }

    bool isConnected() {
        return connected;
    }

private:
    static DWORD WINAPI receiveThreadFunction(LPVOID arg);

    bool receivePacket();

private:
    char buffer[MAX_BUFFER_SIZE];

    std::string server_ip;
    SOCKET socket_fd;
    bool connected;

    void (*callback)(const void *buffer, int buf_len, void* user);
    void* user;

    HANDLE receiveThread;
};



#endif