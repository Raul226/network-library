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
        class client : public logs::error
        {
        public:
            client();
            client(int socket_id);
            ~client();
            void hintSetup(int family, int type, int protocol, int flags);
            bool setSocketAddress(std::string address, std::string port);
            bool createSocket();
            bool connectSocket();
            void sendBuffer(std::string message);
            std::string receiveBuffer(int buffer_size);

        private:
#ifdef _WIN32
            WSADATA wsaData;
#endif
            int socket_id;
            int client_socket_id;
            struct addrinfo hints;
            struct addrinfo *result;
        };
    }
}