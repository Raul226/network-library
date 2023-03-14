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

#include "log.hpp"

#define SHUTDOWN_RECEIVE 0
#define SHUTDOWN_SEND 1
#define SHUTDOWN_BOTH 3

namespace network
{
    namespace tcp
    {
        class connection : public logs::error
        {
        public:
            connection(int socket_fd);
            ~connection();
            unsigned int getSocketFileDescriptor();
            void sendBuffer(char *buffer, unsigned int buffer_size);
            unsigned int receiveBuffer(char *buffer, unsigned int buffer_size);
            std::string getAddress();
            std::string getPort();
            void shutdownSocket(int how);
            void closeSocket();

        private:
            sockaddr_in getSocketData();
#ifdef _WIN32
            WSADATA wsaData;
#endif
            int socket_fd;
        };
    }
}