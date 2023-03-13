#pragma once

#ifdef _WIN32
#include <WS2tcpip.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <string>
#include <cstring>

#include "log.hpp"

#define SHUTDOWN_RECEIVE 0
#define SHUTDOWN_SEND 1
#define SHUTDOWN_BOTH 3

namespace network
{
    namespace udp
    {
        class datagram : public logs::error
        {
        public:
            datagram();
            ~datagram();
            void hintSetup(int family, int flags);
            bool setLocalSocketAddress(std::string port);
            bool createSocket();
            bool bindSocket();
            unsigned int getSocketFileDescriptor();
            void sendBufferTo(std::string address, std::string port, char *buffer, unsigned int buffer_size);
            unsigned int receiveBufferFrom(std::string address, std::string port, char *buffer, unsigned int buffer_size);
            void shutdownSocket(int how);
            void closeSocket();

        private:
#ifdef _WIN32
            WSADATA wsaData;
#endif
            int socket_fd;
            struct addrinfo hints;
            struct addrinfo *result;
        };
    }
}