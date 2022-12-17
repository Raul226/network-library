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
            void sendBufferTo(std::string address, std::string port, char *buffer, unsigned int buffer_size);
            unsigned int receiveBufferFrom(std::string address, std::string port, char *buffer, unsigned int buffer_size);

        private:
#ifdef _WIN32
            WSADATA wsaData;
#endif
            int socket_id;
            struct addrinfo hints;
            struct addrinfo *result;
        };
    }
}