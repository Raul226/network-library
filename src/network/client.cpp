#include "client.hpp"

/**
 * @brief Default constructor for a client object
 * If _WIN32 is defined it calls WSAStartup()
 * Also it sets the bytes of hint structure to 0
 */
network::tcp::client::client()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::tcp::client::wsaData)) != 0)
        this->addError("Cannot initialize ws2!");
#endif
    memset(&(this->hints), 0, sizeof(this->hints));
}

/**
 * @brief Used to close the client socket
 * If _WIN32 is defined it calls WSACleanup()
 */
network::tcp::client::~client()
{
    this->shutdownSocket(SHUTDOWN_BOTH);
    this->closeSocket();
}

/**
 * @brief Used to fill the hints structure
 *
 * @param family Family
 * @param flags Flags
 */
void network::tcp::client::hintSetup(int family, int flags)
{
    this->hints.ai_family = family;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_protocol = IPPROTO_TCP;
    this->hints.ai_flags = flags;
}

/**
 * @brief Fills the result structure using the given address, port
 * @brief and based on the hints specified using hintSetup() method
 *
 * @param address Address
 * @param port Port
 * @return true if the call to getaddrinfo() is successfull
 * @return false if it fails
 */
bool network::tcp::client::setSocketAddress(std::string address, std::string port)
{
    if (getaddrinfo(address.c_str(), port.c_str(), &(this->hints), &(this->result)) != 0)
    {
        this->addError("Cannot get address info!");
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief Creates a socket based on the result from the setSocketAddress() method
 *
 * @return true if the socket has been created
 * @return false if it failed
 */
bool network::tcp::client::createSocket()
{
    this->socket_fd = socket(this->result->ai_family, this->result->ai_socktype, this->result->ai_protocol);
    if (this->socket_fd == -1)
    {
        this->addError("Cannot initialize socket!");
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief Connects to the address and port specified by setSocketAddress() method
 *
 * @return true if it connected
 * @return false if it failed
 */
bool network::tcp::client::connectSocket()
{
    if (connect(this->socket_fd, this->result->ai_addr, this->result->ai_addrlen) == -1)
    {
        this->addError("Cannot connect!");
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief Uses getsockname to get client socket data
 *
 * @return a sockaddr_in struct with the client information
 */
sockaddr_in *network::tcp::client::getSocketData()
{
    sockaddr_in *socket_data;
    socklen_t *socket_data_length;
    getsockname(this->socket_fd, (sockaddr *)socket_data, socket_data_length);
    return socket_data;
}

/**
 * @brief Used to get the client IP address
 *
 * @return the client IP address as a std::string
 */
std::string network::tcp::client::getAddress()
{
    return inet_ntoa(this->getSocketData()->sin_addr);
}

/**
 * @brief Gets the client socket file descriptor
 *
 * @return the client socket file descriptor
 */
unsigned int network::tcp::client::getSocketFileDescriptor()
{
    return this->socket_fd;
}

/**
 * @brief Sends a buffer to the connected socket
 *
 * @param buffer Buffer
 * @param buffer_size Buffer Size
 */
void network::tcp::client::sendBuffer(char *buffer, unsigned int buffer_size)
{
    if (send(this->socket_fd, buffer, buffer_size, 0) == -1)
        this->addError("Cannot send buffer!");
}

/**
 * @brief Receive a buffer from the connection
 *
 * @param buffer Buffer
 * @param buffer_size Buffer Size
 * @return The size of the received buffer
 */
unsigned int network::tcp::client::receiveBuffer(char *buffer, unsigned int buffer_size)
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
 * @brief It shutdown the socket
 *
 * @param how How to shutdown the socket(SHUTDOWN_RECEIVE, SHUTDOWN_SEND or SHUTDOWN_BOTH)
 */
void network::tcp::client::shutdownSocket(int how)
{
    shutdown(this->socket_fd, how);
}

/**
 * @brief It closes the socket file descriptor(on _WIN32 system it calls both closesocket and WSACleanup)
 */
void network::tcp::client::closeSocket()
{
#ifdef _WIN32
    closesocket(this->socket_fd);
    WSACleanup();
#else
    close(this->socket_fd);
#endif
}