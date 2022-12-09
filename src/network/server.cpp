#include "server.hpp"

network::tcp::server::server()
{
    this->errors = new logs::error();
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &(network::tcp::server::wsaData)) != 0)
        this->errors->addError("Cannot initialize ws2!");
#endif
    memset(&(this->hints), 0, sizeof(this->hints));
}

network::tcp::server::~server()
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

void network::tcp::server::hintSetup(int af, int type, int protocol, int flags)
{
    this->hints.ai_family = af;
    this->hints.ai_socktype = type;
    this->hints.ai_protocol = protocol;
    this->hints.ai_flags = flags;
}

bool network::tcp::server::setLocalSocketAddress(std::string port)
{
    if (getaddrinfo(0, port.c_str(), &(this->hints), &(this->result)) != 0)
    {
        this->errors->addError("Cannot get address info!");
        return false;
    }
    else
    {
        return true;
    }
}

bool network::tcp::server::createSocket()
{
    this->socket_id = socket(this->result->ai_family, this->result->ai_socktype, this->result->ai_protocol);
    if (this->socket_id == -1)
    {
        this->errors->addError("Cannot initialize socket!");
        return false;
    }
    else
    {
        return true;
    }
}

bool network::tcp::server::bindSocket()
{
    if (bind(this->socket_id, this->result->ai_addr, this->result->ai_addrlen) == -1)
    {
        this->errors->addError("Cannot bind socket!");
        return false;
    }
    else
    {
        return true;
    }
}

int network::tcp::server::listenSocket()
{
    if (listen(this->socket_id, 0) == -1)
    {
        this->errors->addError("Cannot listen!");
        return 0;
    }
    else
    {
        int client_socket_id = -1;
        client_socket_id = accept(this->socket_id, 0, 0);
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
}