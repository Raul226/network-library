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

#define SHUTDOWN_RECEIVE 0
#define SHUTDOWN_SEND 1
#define SHUTDOWN_BOTH 3

namespace network
{
    namespace udp
    {
        class datagram
        {
        public:
            datagram();
            ~datagram();
            void hintSetup(int family, int flags);
            bool setLocalSocketAddress(std::string port);
            bool createSocket();
            bool bindSocket();
            std::string getAddress();
            std::string getPort();
            bool sendBufferTo(std::string address, std::string port, void *buffer, unsigned int buffer_size);
            unsigned int receiveBufferFrom(std::string address, std::string port, void *buffer, unsigned int buffer_size);
            bool shutdownSocket(int how);
            bool closeSocket();
#ifdef _WIN32
            SOCKET getSocketFileDescriptor();
#else
            int getSocketFileDescriptor();
#endif

        private:
            bool getSocketData(sockaddr_in *socket_data);
#ifdef _WIN32
            WSADATA wsaData;
            SOCKET socket_fd = -1;
#else
            int socket_fd = -1;
#endif
            struct addrinfo hints;
            struct addrinfo *result;
        };
    }
}