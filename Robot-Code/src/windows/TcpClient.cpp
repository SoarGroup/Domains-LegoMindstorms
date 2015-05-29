
#define __USE_W32_SOCKETS
#include <winsock2.h>
#include <WS2tcpip.h>

#include "windows/TcpClient.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

TcpClient::TcpClient(string server_ip)
: server_ip(server_ip), connected(false), callback(0), user(0) {
}

TcpClient::~TcpClient() {
    if (connected) {
        closesocket(socket_fd);
    }
}

bool TcpClient::openConnection() {
	if (connected){
		printf("TcpClient Error: Trying to connect when already connected\n");
		return false;
	}

	// Initiate Winsock Processes 
    WSADATA wsadata;
    int error = WSAStartup(0x0202, &wsadata);
    if (error) {
        printf("TcpClient Error: WSAStartup = %d\n", WSAGetLastError());
        return false;
    }

    if (wsadata.wVersion != 0x0202) {
        printf("TcpClient Error: Bad WSA version\n");
        WSACleanup();
        return false;
    }

	// Create the socket
	printf("TcpClient: creating socket\n");
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd == INVALID_SOCKET) {
        printf("TcpClient Error: socket() = %d\n", WSAGetLastError());
        return false;
    }

	// Setup the server address used to connect
    SOCKADDR_IN server_addr;
    memset(&server_addr, 0, sizeof (server_addr));
    server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, server_ip.c_str(), (void*)&server_addr.sin_addr);
	server_addr.sin_port = htons(DEFAULT_PORT);

	// Connect to the server
    printf("TcpClient: connecting\n");
    if (connect(socket_fd, (SOCKADDR*) & server_addr, sizeof (server_addr)) == SOCKET_ERROR) {
        printf("TcpClient Error: connect = %d\n", WSAGetLastError());
        closesocket(socket_fd);
        return false;
    }

	// Set the TCP_NODELAY flag so messages are immediately sent
    int flag = 1;
    if (setsockopt(socket_fd, IPPROTO_TCP, TCP_NODELAY, (const char*) &flag, sizeof (flag)) == SOCKET_ERROR) {
        printf("TcpClient Error: setsockopt TCP_NODELAY = %d\n", WSAGetLastError());
        closesocket(socket_fd);
        return false;
    }

    connected = true;
    printf("TcpClient: socket is connected\n");

    DWORD threadId;
    receiveThread = CreateThread(NULL, 0, &receiveThreadFunction, (void*) this, 0, &threadId);
    return true;
}

void TcpClient::closeConnection(){
	if (connected){
		connected = false;
		closesocket(socket_fd);
	}
	WaitForSingleObject(receiveThread, INFINITE);
}

DWORD WINAPI TcpClient::receiveThreadFunction(LPVOID arg) {
    TcpClient* server = (TcpClient*) arg;

    while (server->isConnected()){
		server->receivePacket();
    }

    return 0;
}

bool TcpClient::receivePacket() {
    //printf("TcpClient: listening for packet\n");
    int len = recv(socket_fd, buffer, MAX_BUFFER_SIZE - 1, 0);
    if (len < 0) {
        printf("TcpClient Error: recv = %d\n", WSAGetLastError());
		connected = false;
        return false;
    }
    if (len == 0) {
		printf("TcpClient: connection closed\n");
		connected = false;
        return false;
    }
    //printf("TcpClient: received packet of size %d\n", (int)len);
    if (callback != 0) {
        callback(buffer, len, user);
    }
    return true;
}

bool TcpClient::sendPacket(const void* buffer, int buf_len) {
    //printf("TcpClient: sending packet of length %d\n", (int)buf_len);
    if (!connected) {
        printf("TcpClient Error: Trying to send on bad connection\n");
        return false;
    }

    if (send(socket_fd, (char*) buffer, buf_len, 0) == SOCKET_ERROR) {
        printf("TcpClient Error: send = %d\n", WSAGetLastError());
		connected = false;
        return false;
    }
    //printf("TcpClient: successfully sent packet\n");
    return true;
}