#include "client.hpp"

network::tcp::client::client()
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::tcp::client::wsaData)) != 0)
        this->addError("Cannot initialize ws2!");
#endif
    memset(&(this->hints), 0, sizeof(this->hints));
}

network::tcp::client::client(int socket_id)
{
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::tcp::client::wsaData)) != 0)
        this->addError("Cannot initialize ws2!");
#endif
    memset(&(this->hints), 0, sizeof(this->hints));
    this->client_socket_id = socket_id;
}

network::tcp::client::~client()
{
#ifdef _WIN32
    shutdown(this->socket_id, SD_SEND);
    shutdown(this->client_socket_id, SD_SEND);
    closesocket(this->socket_id);
    closesocket(this->client_socket_id);
    WSACleanup();
#else
    shutdown(this->socket_id, SHUT_RDWR);
    shutdown(this->client_socket_id, SHUT_RDWR);
    close(this->socket_id);
    close(this->client_socket_id);
#endif
}

void network::tcp::client::hintSetup(int af, int type, int protocol, int flags)
{
    this->hints.ai_family = af;
    this->hints.ai_socktype = type;
    this->hints.ai_protocol = protocol;
    this->hints.ai_flags = flags;
}

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

bool network::tcp::client::connectSocket()
{
    if (connect(this->socket_id, this->result->ai_addr, this->result->ai_addrlen) == -1)
    {
        this->addError("Cannot connect!");
        return false;
    }
    else
    {
        this->client_socket_id = this->socket_id;
        return true;
    }
}

void network::tcp::client::sendBuffer(std::string message)
{
    const char *buffer = new char[message.length()];
    buffer = message.c_str();
    send(this->client_socket_id, buffer, message.length(), 0);
}

std::string network::tcp::client::receiveBuffer(int buffer_size)
{
    char *buffer = new char[buffer_size];
    recv(this->client_socket_id, buffer, buffer_size, 0);
    return buffer;
}