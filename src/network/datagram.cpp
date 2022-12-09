#include "datagram.hpp"

network::udp::datagram::datagram()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::udp::datagram::wsaData)) != 0)
        this->addError("Cannot initialize ws2!");
#endif
    memset(&(this->hints), 0, sizeof(this->hints));
}

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

void network::udp::datagram::hintSetup(int af, int type, int protocol, int flags)
{
    this->hints.ai_family = af;
    this->hints.ai_socktype = type;
    this->hints.ai_protocol = protocol;
    this->hints.ai_flags = flags;
}

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
        return "error";
    }
    else
    {
        char *buffer = new char[1024];
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
            return "error";
        }
        else
        {
            return buffer;
        }
    }
}