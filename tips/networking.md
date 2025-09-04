---
title: "Lab 11 - Networking"
keywords: Networking
tags: [Networking]
permalink: lab11.html
summary: Networking
sidebar: home_sidebar
---

## Networking with SFML
====================

This will be a fairly short practical focused on showing you how to get up and running with networking in SFML. The lab is based on SFML's own tutorial found here -- [sfml-dev.org/tutorials/2.4/network-socket.php](https://www.sfml-dev.org/tutorials/2.4/network-socket.php)

### CMake Code

Up until now we have not been including the networking part of SFML in our builds. This was just to make builds a bit quicker; they are already quite long. The library we need is `sfml-network`. To use it, we specify this in our `target_link_libraries` attribute in the CMake file.

We are also going to build two applications this week, a client and a server. We are also only going to build console applications. The CMake code we require is below.

```cmake
## 11 - Networking
add_executable(PRACTICAL_11_NETWORKING_SERVER practical_11_networking/server.cpp)
target_link_libraries(PRACTICAL_11_NETWORKING_SERVER sfml-network)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_11_NETWORKING_SERVER)
add_executable(PRACTICAL_11_NETWORKING_CLIENT practical_11_networking/client.cpp)
target_link_libraries(PRACTICAL_11_NETWORKING_CLIENT sfml-network)
set(EXECUTABLES ${EXECUTABLES} PRACTICAL_11_NETWORKING_CLIENT)
```

This will create two executables. The server is the first one we need and will listen for incoming connections. The client is the second application which will connect to the server.

## Starting a Basic Server

SFML networking is easy to get started. The following shows how we create a server application.

```cpp
//server.cpp
#include <iostream>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

int main(int argc, char **argv) {
    // Create a server socket (listener) on port 5000
    TcpListener listener;
    if (listener.listen(5000) != Socket::Done) {
        cout << "Server could not open socket" << endl;
        return -1;
    }
    
    // Accept a new incoming client connection
    TcpSocket client;
    if (listener.accept(client) != Socket::Done) {
        cout << "Server could not accept connection" << endl;
        return -1;
    }
    
    return 0;
}
```

Points of interest are:

-   Line 10 creates a new listening or server socket. The listener will accept a new incoming client connection.
-   On line 11 we choose a port for the listener to listen on. We use 5000 here.
-   On line 19 we we accept a new incoming connection on line 19.

Running this application will start the server and it will wait for an incoming connection. We better build a client to do this.

## Starting a Basic Client

The client is the easier part of the application as it just needs to connect to the server. This is done in the following code.

```cpp
//client.cpp
#include <iostream>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

int main(int argc, char **argv) {
    // Create a socket and connect to server.  Change IP if needed.
    TcpSocket socket;
    Socket::Status status = socket.connect("127.0.0.1", 5000);
    if (status != Socket::Done) {
        cout << "Error - could not connect to server" << endl;
        return -1;
    }
    
    return 0;
}
```

Only line 11 is of interest here. This will connect to an IP address and port. The IP address is the "localhost" (127.0.0.1) but if you are running on different machines you will have to use the IP of the server. The port must match the server's listening port.

If you run the client it will complete as will the server. They have performed the connection operation. If you get errors you will need to resolve them. Otherwise we can move onto sending and receiving data.

## Sending and Receiving

We are going to keep this simple and just send character data from a string. We will read in a message on the server and send it to the client, and the server will then wait for a response. The client will perform the inverse. It will accept a message from the server and then read in a response to send. The two pieces of code required are below.

```cpp
//server.cpp
string msg;
cout << "Enter message for client: ";
cin >> msg;
// Send message to client - c_str is the raw char data in a string.
// Need +1 for length for string null terminator.
if (client.send(msg.c_str(), msg.length() + 1) != Socket::Done) {
    cout << "Error - server sending data" << endl;
    return -1;
}

char buffer[100];
size_t received;
// Receive upto 100 characters from the client.
if (client.receive(buffer, 100, received) != Socket::Done) {
    cout << "Error - server receiving data" << endl;
    return -1;
}
msg = string(buffer);
cout << "Received from client: " << msg << endl;
```

```cpp
//client.cpp
char buffer[100];
size_t received;
if (socket.receive(buffer, 100, received) != Socket::Done) {
    cout << "Error - client receiving data" << endl;
    return -1;
}
string msg(buffer);
cout << "Received from server: " << msg << endl;

cout << "Message for server: ";
cin >> msg;
if (socket.send(msg.c_str(), msg.length() + 1) != Socket::Done) {
    cout << "Error - client sending data" << endl;
    return -1;
}
```

Running the updated server and client should allow you to send simple messages back and forth between the applications. Data can be anything -- we have just used strings for convenience.

## Adding a UDP Server


As mentioned in the lecture, UDP is a connectionless approach which can be better for games. Our server behaviour changes now. It only has to open a port and then accept data. It can also determine who sent the data. This is shown in the following code.

```cpp
//server.cpp
// Open a UDP socket on port 6000.
UdpSocket socket;
if (socket.bind(6000) != Socket::Done) {
    cout << "Error - creating UDP socket" << endl;
    return -1;
}

// Receive data on the socket.
IpAddress sender;
unsigned short port;
if (socket.receive(buffer, 100, received, sender, port) != Socket::Done) {
    cout << "Error - receiving data on UDP port" << endl;
    return -1;
}
msg = string(buffer);
cout << "Received " << msg << " from " << sender << " on port " << port << endl;
```

## Adding a UDP Client

The client can effectively send a UDP message anywhere. It does this by creating a UDP socket and calling send with a server address and port. This is illustrated below.

```cpp
//client.cpp
cout << "UDP message for server: ";
cin >> msg;
// Send via UDP to given IP and socket.
UdpSocket udp;
IpAddress server = "127.0.0.1";
unsigned short port = 6000;
if (udp.send(msg.c_str(), msg.length() + 1, server, port) != Socket::Done)
{
    cout << "Error - client sending via UDP" << endl;
    return -1;
}
```

And that is it. Well sort of. The challenge you have is to integrate this within your game and to look into asynchronous communication. The SFML tutorial link above provides details. We have given you the starting point and this should be enough to understand the other methods.

Previous step: [CI](lab10_2)

Next step: NONE! You're finished! Well done!