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
    namespace tcp
    {
        class server
        {
        public:
            server();
            ~server();
            void hintSetup(int family, int flags);
            bool setLocalSocketAddress(std::string port);
            bool createSocket();
            bool bindSocket();
            bool listenSocket();
            int acceptConnection();
            std::string getAddress();
            std::string getPort();
            unsigned int getSocketFileDescriptor();
            bool shutdownSocket(int how);
            bool closeSocket();

        private:
            bool getSocketData(sockaddr_in *socket_data);
#ifdef _WIN32
            WSADATA wsaData;
#endif
            int socket_fd = -1;
            struct addrinfo hints;
            struct addrinfo *result;
        };
    }
}