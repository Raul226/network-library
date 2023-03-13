#include "datagram.hpp"

/**
 * @brief Default constructor for a datagram object
 * If _WIN32 is defined it calls WSAStartup()
 * Also it sets the bytes of hint structure to 0
 */
network::udp::datagram::datagram()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::udp::datagram::wsaData)) != 0)
        this->addError("Cannot initialize ws2!");
#endif
    memset(&(this->hints), 0, sizeof(this->hints));
}

/**
 * @brief Used to close the datagram socket
 * If _WIN32 is defined it calls WSACleanup()
 */
network::udp::datagram::~datagram()
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
void network::udp::datagram::hintSetup(int family, int flags)
{
    this->hints.ai_family = family;
    this->hints.ai_socktype = SOCK_DGRAM;
    this->hints.ai_protocol = IPPROTO_UDP;
    this->hints.ai_flags = flags;
}

/**
 * @brief Gets the local machine address and fill the result structure
 * @brief based on the hints specified using hintSetup() method
 *
 * @param port Port
 * @return true if the call to getaddrinfo() is successfull
 * @return false if it fails
 */
bool network::udp::datagram::setLocalSocketAddress(std::string port)
{
    if (getaddrinfo(0, port.c_str(), &(this->hints), &(this->result)) != 0)
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
 * @brief Creates a socket based on the result from the setLocalSocketAddress() method
 *
 * @return true if the socket has been created
 * @return false if it failed
 */
bool network::udp::datagram::createSocket()
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
 * @brief Binds the socket to the address and port from result
 *
 * @return true if the socket has been bound successfully
 * @return false if it failed
 */
bool network::udp::datagram::bindSocket()
{
    if (bind(this->socket_fd, this->result->ai_addr, this->result->ai_addrlen) == -1)
    {
        this->addError("Cannot bind socket!");
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * @brief Gets the datagram socket file descriptor
 *
 * @return the datagram socket file descriptor
 */
unsigned int network::udp::datagram::getSocketFileDescriptor()
{
    return this->socket_fd;
}

/**
 * @brief Sends a buffer to the specified address and port
 *
 * @param address Address
 * @param port Port
 * @param buffer Buffer
 * @param buffer_size Buffer Size
 */
void network::udp::datagram::sendBufferTo(std::string address, std::string port, char *buffer, unsigned int buffer_size)
{
    struct addrinfo *udp_addrinfo_result;
    struct addrinfo udp_addrinfo_hints;

    memset(&udp_addrinfo_hints, 0, sizeof(udp_addrinfo_hints));

    udp_addrinfo_hints.ai_family = AF_INET;
    udp_addrinfo_hints.ai_socktype = SOCK_DGRAM;
    udp_addrinfo_hints.ai_protocol = IPPROTO_UDP;
    udp_addrinfo_hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(address.c_str(), port.c_str(), &udp_addrinfo_hints, &udp_addrinfo_result) != 0)
    {
        this->addError("Cannot get udp addrinfo!");
    }
    else
    {
        if (sendto(this->socket_fd, buffer, buffer_size, 0, udp_addrinfo_result->ai_addr, udp_addrinfo_result->ai_addrlen) == -1)
            this->addError("Cannot send buffer!");
    }
}

/**
 * @brief Receives a buffer from the specified address and port
 *
 * @param address Address
 * @param port Port
 * @param buffer Buffer
 * @param buffer_size Buffer Size
 * @return The size of the received buffer
 */
unsigned int network::udp::datagram::receiveBufferFrom(std::string address, std::string port, char *buffer, unsigned int buffer_size)
{
    struct addrinfo *udp_addrinfo_result;
    struct addrinfo udp_addrinfo_hints;

    memset(&udp_addrinfo_hints, 0, sizeof(udp_addrinfo_hints));

    udp_addrinfo_hints.ai_family = AF_INET;
    udp_addrinfo_hints.ai_socktype = SOCK_DGRAM;
    udp_addrinfo_hints.ai_protocol = IPPROTO_UDP;
    udp_addrinfo_hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(address.c_str(), port.c_str(), &udp_addrinfo_hints, &udp_addrinfo_result) != 0)
    {
        this->addError("Cannot get udp addrinfo!");
        return 0;
    }
    else
    {
        memset(buffer, 0, buffer_size);
        int receive = -1;
#ifdef _WIN32
        int addrinfo_len = (int)udp_addrinfo_result->ai_addrlen;
        receive = recvfrom(this->socket_fd, buffer, buffer_size, 0, udp_addrinfo_result->ai_addr, &addrinfo_len);
#else
        receive = recvfrom(this->socket_fd, buffer, buffer_size, 0, udp_addrinfo_result->ai_addr, &udp_addrinfo_result->ai_addrlen);
#endif
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
}

/**
 * @brief It shutdown the socket
 *
 * @param how How to shutdown the socket(SHUTDOWN_RECEIVE, SHUTDOWN_SEND or SHUTDOWN_BOTH)
 */
void network::udp::datagram::shutdownSocket(int how)
{
    shutdown(this->socket_fd, how);
}

/**
 * @brief It closes the socket file descriptor(on _WIN32 system it calls both closesocket and WSACleanup)
 */
void network::udp::datagram::closeSocket()
{
#ifdef _WIN32
    closesocket(this->socket_fd);
    WSACleanup();
#else
    close(this->socket_fd);
#endif
}