#include "connection.hpp"

/**
 * @brief Creates a network client connection object using the socket file descriptor received
 *
 * @param socket_id Socket file descriptor
 */
network::tcp::connection::connection(int socket_id)
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::tcp::connection::wsaData)) != 0)
        this->addError("Cannot initialize ws2!");
#endif
    this->socket_id = socket_id;
}

/**
 * @brief Used to close the client connection socket
 * If _WIN32 is defined it calls WSACleanup()
 */
network::tcp::connection::~connection()
{
    this->shutdownSocket(SHUTDOWN_BOTH);
    this->closeSocket();
}

/**
 * @brief Sends a buffer to the connected socket
 *
 * @param message Buffer
 */
void network::tcp::connection::sendBuffer(char *buffer, unsigned int buffer_size)
{
    if (send(this->socket_id, buffer, buffer_size, 0) == -1)
        this->addError("Cannot send buffer!");
}

/**
 * @brief Waiting for a buffer from the connection
 *
 * @param buffer_size Buffer Size
 * @return The size of the received buffer
 */
unsigned int network::tcp::connection::receiveBuffer(char *buffer, unsigned int buffer_size)
{
    memset(buffer, 0, buffer_size);
    int receive = recv(this->socket_id, buffer, buffer_size, 0);
    if (receive == -1)
    {
        this->addError("Cannot receive buffer");
        return 0;
    }
    else
    {
        return receive;
    }
}

/**
 * @brief It shutdown the socket
 *
 * @param how How to shutdown the socket(SHUTDOWN_RECEIVE, SHUTDOWN_SEND or SHUTDOWN_BOTH)
 */
void network::tcp::connection::shutdownSocket(int how)
{
    shutdown(this->socket_id, how);
}

/**
 * @brief It closes the socket file descriptor(on _WIN32 system it calls both closesocket and WSACleanup)
 */
void network::tcp::connection::closeSocket()
{
#ifdef _WIN32
    closesocket(this->socket_id);
    WSACleanup();
#else
    close(this->socket_id);
#endif
}