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
 */
void network::tcp::connection::sendBuffer(char *buffer, unsigned int buffer_size)
{
    if (send(this->socket_fd, buffer, buffer_size, 0) == -1)
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
    int receive = recv(this->socket_fd, buffer, buffer_size, 0);
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
 * @brief Uses getsockname to get connection socket data
 *
 * @return a sockaddr_in struct with the connection information
 */
sockaddr_in network::tcp::connection::getSocketData()
{
    sockaddr_in socket_data;
    memset(&socket_data, 0, sizeof(socket_data));
    socklen_t socket_data_length = sizeof(socket_data);
    if (getpeername(this->socket_fd, (sockaddr *)&socket_data, &socket_data_length) != 0)
        this->addError("Cannot get socket data");

    return socket_data;
}

/**
 * @brief Used to get the connection IP address
 *
 * @return the connection IP address as a std::string
 */
std::string network::tcp::connection::getAddress()
{
    return inet_ntoa(this->getSocketData().sin_addr);
}

/**
 * @brief Used to get the connection port
 *
 * @return the connection port as a std::string
 */
std::string network::tcp::connection::getPort()
{
    return std::to_string(this->getSocketData().sin_port);
}
/**
 * @brief It shutdown the socket
 *
 * @param how How to shutdown the socket(SHUTDOWN_RECEIVE, SHUTDOWN_SEND or SHUTDOWN_BOTH)
 */
void network::tcp::connection::shutdownSocket(int how)
{
    shutdown(this->socket_fd, how);
}

/**
 * @brief It closes the socket file descriptor(on _WIN32 system it calls both closesocket and WSACleanup)
 */
void network::tcp::connection::closeSocket()
{
#ifdef _WIN32
    closesocket(this->socket_fd);
    WSACleanup();
#else
    close(this->socket_fd);
#endif
}