#include "connection.hpp"

/**
 * @brief Creates a network client connection object using the socket file descriptor received
 *
 * @param socket_fd Socket file descriptor
 */
network::tcp::connection::connection(int socket_fd)
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::tcp::connection::wsaData)) != 0)
        this->addError("Cannot initialize ws2!");
#endif
    this->socket_fd = socket_fd;
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
 * @brief Gets the connection socket file descriptor
 *
 * @return the connection socket file descriptor
 */
unsigned int network::tcp::connection::getSocketFileDescriptor()
{
    return this->socket_fd;
}

/**
 * @brief Sends a buffer to the connected socket
 *
 * @param message Buffer
 * @return true if the buffer was sent
 * @return false if it failed
 */
bool network::tcp::connection::sendBuffer(unsigned char *buffer, unsigned int buffer_size)
{
    if (send(this->socket_fd, buffer, buffer_size, 0) == -1)
        return false;
    else
        return true;
}

/**
 * @brief Waiting for a buffer from the connection
 *
 * @param buffer_size Buffer Size
 * @return The size of the received buffer
 * @return -1 if it failed
 */
unsigned int network::tcp::connection::receiveBuffer(unsigned char *buffer, unsigned int buffer_size)
{
    memset(buffer, 0, buffer_size);
    int receive = recv(this->socket_fd, buffer, buffer_size, 0);
    if (receive == -1)
        return -1;
    else
        return receive;
}

/**
 * @brief Uses getsockname to get connection socket data
 *
 * @param socket_data the pointer to the structure where the socket data will get saved
 * @return true if socket data was saved
 * @return false if it failed
 */
bool network::tcp::connection::getSocketData(sockaddr_in *socket_data)
{
    memset(socket_data, 0, sizeof(sockaddr_in));
    socklen_t socket_data_length = sizeof(sockaddr_in);
    if (getpeername(this->socket_fd, (sockaddr *)socket_data, &socket_data_length) == -1)
        return false;
    else
        return true;
}

/**
 * @brief Used to get the connection IP address
 *
 * @return the connection IP address as a std::string
 */
std::string network::tcp::connection::getAddress()
{
    sockaddr_in socket_data;
    if (this->getSocketData(&socket_data))
        return inet_ntoa(socket_data.sin_addr);
    else
        return "";
}

/**
 * @brief Used to get the connection port
 *
 * @return the connection port as a std::string
 */
std::string network::tcp::connection::getPort()
{
    sockaddr_in socket_data;
    if (this->getSocketData(&socket_data))
        return std::to_string(ntohs(socket_data.sin_port));
    else
        return "";
}
/**
 * @brief It shutdown the socket
 *
 * @param how How to shutdown the socket(SHUTDOWN_RECEIVE, SHUTDOWN_SEND or SHUTDOWN_BOTH)
 * @return true if the socket was shutdown
 * @return false if it failed
 */
bool network::tcp::connection::shutdownSocket(int how)
{
    if (shutdown(this->socket_fd, how) == -1)
        return false;
    else
        return true;
}

/**
 * @brief It closes the socket file descriptor(on _WIN32 system it calls both closesocket and WSACleanup)
 *
 * @return true if the socket was closed
 * @return false if it failed
 */
bool network::tcp::connection::closeSocket()
{
#ifdef _WIN32
    if (closesocket(this->socket_fd) == -1 || WSACleanup() == -1)
        return false;
    else
        return true;
#else
    if (close(this->socket_fd) == -1)
        return false;
    else
        return true;
#endif
}