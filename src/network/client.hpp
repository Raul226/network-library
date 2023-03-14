#pragma once

#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netdb.h>
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
        class client : public logs::error
        {
        public:
            client();
            ~client();
            void hintSetup(int family, int flags);
            bool setSocketAddress(std::string address, std::string port);
            bool createSocket();
            bool connectSocket();
            std::string getAddress();
            std::string getPort();
            unsigned int getSocketFileDescriptor();
            void sendBuffer(char *buffer, unsigned int buffer_size);
            unsigned int receiveBuffer(char *buffer, unsigned int buffer_size);
            void shutdownSocket(int how);
            void closeSocket();

        private:
            sockaddr_in *getSocketData();
#ifdef _WIN32
            WSADATA wsaData;
#endif
            int socket_fd;
            struct addrinfo hints;
            struct addrinfo *result;
        };
    }
}