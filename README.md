# network-library
Simple, cross-platform _(winsock2/socket)_ C++ TCP/UDP network library.

There are 4 classes available:
  - server (TCP)
  - client (TCP)
  - connection (TCP)
  - datagram (UDP)

## Server Class - TCP

By creating a **server** object you can bind the socket to a specific port on the local machine and start listening for connection.
After accepting any connection you'll get a socket file descriptor that can be passed to the constructor of a new **connection** object.
Using the new connection instance you can send and receive buffers.

## Client Class - TCP

By creating a **client** object you can connect to a server by specifying the address and the port. After that you can use the **client** object to send and receive buffers.

## Connection Class - TCP

**Connection** object are used by the server to comunicate with the connected clients.
You cannot create a **connection** object without having a valid socket file descriptor.
To get a socket file descriptor you need to accept a connection on a **server** object

## Datagram Class - UDP

To create a UDP datagram server you need to bind the object to a port on the local machine.
Otherwise, to use the socket as a UDP client you don't have to bind it, you can directly send/receive packets.

## Example TCP Client/Server

**server.cpp**
```C++
#include <network.hpp>

#define MAX_BUFFER_SIZE 1024

int main()
{
    network::tcp::server *netServer = new network::tcp::server();
    network::tcp::connection *netClient;

    unsigned char *buffer = new unsigned char[MAX_BUFFER_SIZE];

    netServer->hintSetup(AF_INET, AI_PASSIVE);
    netServer->setLocalSocketAddress("3000");
    netServer->createSocket();
    netServer->bindSocket();
    netServer->listenSocket();

    netClient = new network::tcp::connection(netServer->acceptConnection());

    unsigned int length = netClient->receiveBuffer(buffer, MAX_BUFFER_SIZE);
    netClient->sendBuffer(buffer, MAX_BUFFER_SIZE);

    netClient->shutdownSocket(SHUTDOWN_BOTH);
    netClient->closeSocket();

    netServer->shutdownSocket(SHUTDOWN_BOTH);
    netServer->closeSocket();

    return 0;
}
```

**client.cpp**
```C++
#include <network.hpp>

#define MAX_BUFFER_SIZE 1024

int main()
{
    network::tcp::client *netClient = new network::tcp::client();

    unsigned char *buffer = new unsigned char[MAX_BUFFER_SIZE];
    strcpy(buffer, "Hello World!");

    netClient->hintSetup(AF_INET, AI_PASSIVE);
    netClient->setSocketAddress("127.0.0.1", "3000");
    netClient->createSocket();
    netClient->connectSocket();

    netClient->sendBuffer(buffer, MAX_BUFFER_SIZE);

    memset(buffer, 0x0, MAX_BUFFER_SIZE);
    unsigned int length = netClient->receiveBuffer(buffer, MAX_BUFFER_SIZE);

    netClient->shutdownSocket(SHUTDOWN_BOTH);
    netClient->closeSocket();

    return 0;
}
```

## Example UDP Client/Server

**server.cpp**
```C++
#include <network.hpp>

#define MAX_BUFFER_SIZE 1024

int main()
{
    network::udp::datagram *netServer = new network::udp::datagram();

    unsigned char *buffer = new unsigned char[MAX_BUFFER_SIZE];

    netServer->hintSetup(AF_INET, AI_PASSIVE);
    netServer->setLocalSocketAddress("3000");
    netServer->createSocket();
    netServer->bindSocket();

    unsigned int length = netServer->receiveBufferFrom("127.0.0.1", "3001", buffer, MAX_BUFFER_SIZE);

    netServer->sendBufferTo("127.0.0.1", "3001", buffer, MAX_BUFFER_SIZE);

    netServer->shutdownSocket(SHUTDOWN_BOTH);
    netServer->closeSocket();

    return 0;
}
```

**client.cpp**
```C++
#include <network.hpp>

#define MAX_BUFFER_SIZE 1024

int main()
{
    network::udp::datagram *netClient = new network::udp::datagram();

    unsigned char *buffer = new unsigned char[MAX_BUFFER_SIZE];

    strcpy(buffer, "Hello World!");

    netClient->hintSetup(AF_INET, AI_PASSIVE);
    netClient->setLocalSocketAddress("3001");
    netClient->createSocket();
    netClient->bindSocket();

    netClient->sendBufferTo("127.0.0.1", "3000", buffer, MAX_BUFFER_SIZE);

    unsigned int length = netClient->receiveBufferFrom("127.0.0.1", "3000", buffer, MAX_BUFFER_SIZE);

    netClient->shutdownSocket(SHUTDOWN_BOTH);
    netClient->closeSocket();

    return 0;
}
```
