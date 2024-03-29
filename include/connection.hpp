#pragma once

#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include <string>
#include <cstring>

#define SHUTDOWN_RECEIVE 0
#define SHUTDOWN_SEND 1
#define SHUTDOWN_BOTH 3

namespace network
{
    namespace tcp
    {
        class connection
        {
        public:
#ifdef _WIN32
            connection(SOCKET socket_fd);
            SOCKET getSocketFileDescriptor();
#else
            connection(int socket_fd);
            int getSocketFileDescriptor();
#endif
            ~connection();
            bool sendBuffer(void *buffer, unsigned int buffer_size);
            unsigned int receiveBuffer(void *buffer, unsigned int buffer_size);
            std::string getAddress();
            std::string getPort();
            bool shutdownSocket(int how);
            bool closeSocket();

        private:
            bool getSocketData(sockaddr_in *socket_data);
#ifdef _WIN32
            WSADATA wsaData;
            SOCKET socket_fd = -1;
#else
            int socket_fd = -1;
#endif
        };
    }
}