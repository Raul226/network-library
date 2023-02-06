#include "server.hpp"

/**
 * @brief Default constructor for a server object
 * If _WIN32 is defined it calls WSAStartup()
 * Also it sets the bytes of hint structure to 0
 */
network::tcp::server::server()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::tcp::server::wsaData)) != 0)
        this->addError("Cannot initialize ws2!");
#endif
    memset(&(this->hints), 0, sizeof(this->hints));
}

/**
 * @brief Used to close the server socket
 * It calls both shutdownSocket and closeSocket
 */
network::tcp::server::~server()
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
void network::tcp::server::hintSetup(int family, int flags)
{
    this->hints.ai_family = family;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_protocol = IPPROTO_TCP;
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
bool network::tcp::server::setLocalSocketAddress(std::string port)
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
bool network::tcp::server::createSocket()
{
    this->socket_id = socket(this->result->ai_family, this->result->ai_socktype, this->result->ai_protocol);
    if (this->socket_id == -1)
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
bool network::tcp::server::bindSocket()
{
    if (bind(this->socket_id, this->result->ai_addr, this->result->ai_addrlen) == -1)
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
 * @brief Puts the socket into listening
 *
 */
void network::tcp::server::listenSocket()
{
    if (listen(this->socket_id, 0) == -1)
        this->addError("Cannot listen!");
}

/**
 * @brief Waits and accept any connection
 *
 * @return The connection socket file descriptor
 */
int network::tcp::server::acceptConnection()
{
    int client_socket_id = accept(this->socket_id, 0, 0);
    if (client_socket_id != -1)
    {
        return client_socket_id;
    }
    else
    {
        return 0;
    }
    return 0;
}

/**
 * @brief It shutdown the socket
 *
 * @param how How to shutdown the socket(SHUTDOWN_RECEIVE, SHUTDOWN_SEND or SHUTDOWN_BOTH)
 */
void network::tcp::server::shutdownSocket(int how)
{
    shutdown(this->socket_id, how);
}

/**
 * @brief It closes the socket file descriptor(on _WIN32 system it calls both closesocket and WSACleanup)
 */
void network::tcp::server::closeSocket()
{
#ifdef _WIN32
    closesocket(this->socket_id);
    WSACleanup();
#else
    close(this->socket_id);
#endif
}