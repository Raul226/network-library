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
    namespace tcp
    {
        class server
        {
        public:
            server();
            ~server();
            void hintSetup(int domain, int type, int protocol, int flags);
            bool setLocalSocketAddress(std::string port);
            bool createSocket();
            bool bindSocket();
            int listenSocket();

        private:
#ifdef _WIN32
            WSADATA wsaData;
#endif
            int socket_id;
            struct addrinfo hints;
            struct addrinfo *result;
            logs::error *errors;
        };
    }
}