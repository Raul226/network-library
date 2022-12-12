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
 * @param af Family
 * @param type Type
 * @param protocol Protocol
 * @param flags Flags
 */
void network::udp::datagram::hintSetup(int af, int type, int protocol, int flags)
{
    this->hints.ai_family = af;
    this->hints.ai_socktype = type;
    this->hints.ai_protocol = protocol;
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
bool network::udp::datagram::bindSocket()
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
 * @brief Sends a buffer to the specified address and port
 *
 * @param address Address
 * @param port Port
 * @param message Buffer
 */
void network::udp::datagram::sendBufferTo(std::string address, std::string port, std::string message)
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
        const char *buffer = new char[message.length()];
        buffer = message.c_str();
        if (sendto(this->socket_id, buffer, message.length(), 0, udp_addrinfo_result->ai_addr, udp_addrinfo_result->ai_addrlen) == -1)
            this->addError("Cannot send buffer!");
    }
}

/**
 * @brief Receive a buffer from the specified address and port
 *
 * @param address Address
 * @param port Port
 * @param buffer_size Buffer size
 * @return The buffer it gets from the specified address, or an empty string if any errors occur
 */
std::string network::udp::datagram::receiveBufferFrom(std::string address, std::string port, int buffer_size)
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
        return "";
    }
    else
    {
        char *buffer = new char[buffer_size];
        memset(buffer, 0, buffer_size);
        int receive = -1;
#ifdef _WIN32
        int addrinfo_len = (int)udp_addrinfo_result->ai_addrlen;
        receive = recvfrom(this->socket_id, buffer, buffer_size, 0, udp_addrinfo_result->ai_addr, &addrinfo_len);
#else
        receive = recvfrom(this->socket_id, buffer, buffer_size, 0, udp_addrinfo_result->ai_addr, &udp_addrinfo_result->ai_addrlen);
#endif
        if (receive == -1)
        {
            this->addError("Cannot receive buffer");
            return "";
        }
        else
        {
            return buffer;
        }
    }
}