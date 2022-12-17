# network-library
Simple, cross-platform C++ TCP/UDP network library.

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

## Datagram - UDP

To create a UDP datagram server you need to bind the object to a port on the local machine.
Otherwise, to use the socket as a UDP client you don't have to bind it, you can directly send/receive packets.
