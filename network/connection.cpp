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
#ifdef _WIN32
    shutdown(this->socket_id, SD_SEND);
    closesocket(this->socket_id);
    WSACleanup();
#else
    shutdown(this->socket_id, SHUT_RDWR);
    close(this->socket_id);
#endif
}

/**
 * @brief Sends a buffer to the connected socket
 *
 * @param message Buffer
 */
void network::tcp::connection::sendBuffer(std::string message)
{
    const char *buffer = new char[message.length()];
    buffer = message.c_str();
    if (send(this->socket_id, buffer, message.length(), 0) == -1)
        this->addError("Cannot send buffer!");
}

/**
 * @brief Waiting for a buffer from the connection
 *
 * @param buffer_size Buffer size in byte
 * @return The buffer it gets from the connected socket, or an empty string if any errors occur
 */
std::string network::tcp::connection::receiveBuffer(int buffer_size)
{
    char *buffer = new char[buffer_size];
    memset(buffer, 0, buffer_size);
    if (recv(this->socket_id, buffer, buffer_size, 0) == -1)
    {
        this->addError("Cannot receive buffer");
        return "";
    }
    else
    {
        return buffer;
    }
}