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
    namespace tcp
    {
        class connection : public logs::error
        {
        public:
            connection(int socket_id);
            ~connection();
            void sendBuffer(char *buffer, unsigned int buffer_size);
            unsigned int receiveBuffer(char *buffer, unsigned int buffer_size);
            void shutdownSocket(int how);
            void closeSocket();

        private:
#ifdef _WIN32
            WSADATA wsaData;
#endif
            int socket_id;
        };
    }
}