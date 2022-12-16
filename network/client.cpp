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
 * @brief Creates a socket based on the result from the setLocalSocketAddress() method
 *
 * @return true if the socket has been created
 * @return false if it failed
 */
bool network::tcp::client::createSocket()
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
 * @brief Connects to the address and port specified by setSocketAddress() method
 *
 * @return true if it connected
 * @return false if it failed
 */
bool network::tcp::client::connectSocket()
{
    if (connect(this->socket_id, this->result->ai_addr, this->result->ai_addrlen) == -1)
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
 * @brief Sends a buffer to the connected socket
 *
 * @param message Buffer
 */
void network::tcp::client::sendBuffer(std::string message)
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
 * @return The buffer it gets from the specified address, or an empty string if any errors occur
 */
std::string network::tcp::client::receiveBuffer(int buffer_size)
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