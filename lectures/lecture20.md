---
title: "Lecture20"
keywords: Lecture
tags: [Lecture]
permalink:  lecture20.html
summary: lecture21
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 20 - Networking
### SET09121 - Games Engineering

<br><br>
Babis Koniaris
<br>


School of Computing. Edinburgh Napier University


---

# Our Goal

- We want to enable our game to support multiple players from different machines.
- This requires some form of communication between the different machines.
- It also requires some form of coordination between the different instances of the game.
- We also have to do this in real-time so that the game does not hang or become unplayable due to lag.
- So how do we achieve this?


---

# Questions

- How does networking work?
- How do we do network programming?
- What are the limitations of networking?
- How do we solve these limitations in games?
- What data should we send in a game and how do we send it?
- How can we do networking in SFML?


---

## How does networking work?

---

# Hardware

- We can break-down network hardware into the following:
    - Computers on the network.
    - Network interfaces on the computers.
    - Routers and switches that interfaces connect to.
    - The connection between the interfaces and the switch/router.

- Each one of these works at a different level of data abstraction.


 ![image](assets/images/network-diagram.png)


---

# The Seven Layer Model

- Networking works on a layered model.
    - Application data at the top layer.
    - Electrical signals at the bottom layer.
- As programmers, we rarely need to consider below level 3.


![image](assets/images/osi_layers.jpg) <!-- .element width="95%"  -->


---

# Packets

- Layer 7 :   your browser works with HTTP.
- Layer 6 :   defines the Secure Socket Layer (SSL) now standard for HTTPS.
- Layer 5 :   defines ports (e.g. port 80 for web traffic).
- Layer 4 :   defines TCP - interface between application and hardware. Breaks data sent into lots of individual packets.
- Layer 3 :   defines IP - allows individual machines to be addressed.
- Layer 2 :   defines MAC addresses - allows the switch to identify physical machines.
- Layer 1 :   actual electrical current between the switch and machine (e.g. cable or wireless).


---

# Addressing and Switching

- Networking works like a mail system.
- Each individual packet has an address (IP address) which the switch has to look at.
- The switch must work out if it can send the data directly to the addressed machine (on the local network) or where it must send it if not.
- The switch has a lookup table matching the local IP addresses to MAC addresses. This is how the switch sees the world.
- The more switches a packet must pass through, the more hops it makes. A packet has a maximum number of hops. If it doesn't make it it dies.
- This is how the entire Internet works (well, kinda).


---

# Transport Control Protocol - TCP

- TCP provides means of breaking down data into separate packets.
- Each data chunk is given a sequence number to allow the data to be reformed.
- TCP guarantees acknowledgement of sent data - this means we know that data has arrived if we don't get an error.
- TCP is the most common distributed application protocol because of its guarantees.


---

## How do we do network programming?


---

# Client-server Model

- The most common application model in networking is the client-server.
- A client connects to a server machine via some form of universal addressing.
- The sever can now communicate directly with that client.
- A server may have multiple clients that it is communicating with.


 ![image](assets/images/client-server.png) <!-- .element width="60%"  -->



---

# Sockets

- Applications communicate using sockets.
- A socket is just an encapsulation of the following:
    - Address: typically an IP address.
    - Protocol: for example TCP.
    - Port: allows an individual application to be addressed on the network.
- These are provided by layers 3-5 of our model.
- A socket is therefore a software abstraction that allows an application to send and receive data with other applications.
- Each socket thus has both a source port and a destination port.


---

# Server Socket

- A server socket is a special type of socket that listens for an initial communication.
- A client socket will connect to this socket initially.
- Once connected, the server creates a new socket exclusively for that client connection (with an appropriate port).
- The server socket can continue listening on its original socket for new incoming connections.
- A server socket will be the first established socket in a distributed application. It is needed to initiate a connection.



---

# Three-way Handshake

- As TCP requires guaranteed connection, the client-server communication initiates what is known as a three-way handshake.
- The client sends a SYN packet to the server.
- The server sends a SYN+ACK packet to acknowledge.
- The client sends a ACK packet to acknowledge.
- Communication is now established.


 ![image](assets/images/three-way.png)


---

## What are the limitations of networking?



---

# Bandwidth and Throughput

- The biggest limitation on a network is its bandwidth.
- The bandwidth indicates the theoretical limit that data can be sent through the network.
    - Typically measured in megabits (1 million bits) per second.
- Most people have heard the term bandwidth, but the actual figure we are interested in is throughput.
- Throughput is the **actual** amount of data that is sent between two machines.
- Throughput will be lower than bandwidth due to limitations between the two machines and other factors.
- Generally, throughput is too low for transferring an entire game's data in 16ms.
    - On a 100Mbps network that is enough time to send 2 kilobytes of data.


---

# Latency

- Latency (or lag) is a bigger concern in games.
- Latency is the time it takes a packet to get to another machine.
- You can find latency easily using ping and dividing the result by two (as ping does a round trip).
- For example, as of writing I can ping Google from my office machine in about 23ms, so latency is about 12ms.
- Latency is therefore going to mean any update you send between game instances will likely be at least one frame out-of-date.


---

# TCP Guarantees

- TCP can be a very slow protocol.
- Each TCP packet must be acknowledged by the receiver.
- The receiver has to rebuild the sent data from the packets.
- If any packet is missing, the entire data is resent.
- If an acknowledgement is not received, the sender resends the data.
- So TCP guarantees come at a cost.


---

## How do we solve these limitations in games?


---

# Peer-to-peer Lockstep

- The original approach to solving networking for games was a peer-to-peer lockstep.
- Here, each client would update it's move to the other clients.
- The game would wait until everyone has updated before moving onto the next move.
- It was slow as you can guess.


 ![image](assets/images/p2p.png)



---

# Client-side Prediction

- Nowadays games will try and predict movement.
- Given that in a single "server" tick we know what the client has moved, we can interpolate the move by the next tick.
- Propagate this amongst all clients and we should have a close approximation to reality.
- Every so often send more complete data.


 ![image](assets/images/client-prediction.png)



---

# User Datagram Packet - UDP

- As we are now predicting movements and locations, we can occasionally lose information without much concern.
- Therefore, we do not need packet guarantees. TCP is no longer needed.
- UDP is an alternate protocol which is connectionless. We just send data to a location.
- The receiver will read its data packet as often as it can, and only those it receives.
- Basically, we can improve performance considerably by not acknowledging data sends.


---

# TCP

![image](assets/images/packet.jpg) <!-- .element width="60%"  -->


---

# UDP

![image](assets/images/udp_packet.png) <!-- .element width="80%"  -->


---

# TCP

![image](assets/images/tcp_joke.png) <!-- .element width="60%"  -->


---

# Synchronous versus Asynchronous Polling

- We can also change how we service connections.
- Typically, we wait on a connection until data arrives.
- This would impact our frame time - we need to service the connection only when data is ready.
- Asynchronous socket communication means we perform an operation whenever a communication event occurs.
- This allows the game to continue on and another thread can update as needed based on communication.



---

# Physics

- A big problem in games actually comes from the physics system.
- The physics system is keeping track of all the physical objects and their interactions.
- A physics engine will typically add some randomness to reactions just to smooth out some of the operations.
- We cannot have this in different clients as it would lead to different game instances having different object locations.
- Solving game physics problems is a whole other area that we won't cover - just send the complete physical data every so often to get around this.


---

## What data should we send in a game and how do we send it?




---

# Initial State

- When a scene is loaded or started, the client needs to know as much of the game world as possible.
- This will require sending a lot of data to the clients when they start.
- This is OK as the game hasn't really started for them yet.
- It also allows other information such as unique content to be shared if need be (although this is slow).
- Use this loading time appropriately.


---

# Scene Updates

- Every "server" tick you need to communicate data between the client and server.
- The communication must update the server with information the client has on scene updates.
- The communication must update the client with information the server has on scene updates.
- Every so often, the client and server must do a more complete update to normalise their information.
- Effectively, we are trying to keep the client and server as synchronised as possible without performing lockstep.


---

# Object Serialization

- When it comes to sending data in object-oriented languages serialization is the go-to approach.
- Serialization is a technique where an object is converted into a textual or binary representation for communication and storage.
- The managed languages (e.g. Java and C#) have these capabilities built in.
- C++ does not, and actually it is hard to convert local memory between two machines.
- Serialization is also a slow process as it has to deeply interrogate the object.
- Therefore, serialization is not a good approach for games in general.


---

# Designing a Protocol

- A better technique is to build a protocol.
- Design your message types and the data that will go into the message.
- This will allow simple messaging that can be managed easily with a few case statements.


 ![image](assets/images/protocol.png)


---

## How can we do networking in SFML?

---

# SFML Networking
- Networking in SFML is relatively easy.
- We will use the following classes:
 - **TcpListener** - a listening or server socket.
 - **TcpSocket** - a socket to communicate via.
 - **UdpSocket** - a UDP socket to communicate via.
- We will use the following methods:
 - **listen** - listen for a new connection.
 - **accept** - accept a new connection.
 - **connect** - connect to a server.
 - **send** - send data via a socket.
 - **receive** - receive data from a socket.



---

# SFML Networking Server

```cpp
#include <iostream>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

int main(int argc, char **argv) {
    TcpListener listener;
    if (listener.listen(5000) != Socket::Done) {
        cout << "Server could not open socket" << endl;
        return -1;
    }
    
    TcpSocket client;
    if (listener.accept(client) != Socket::Done) {
        cout << "Server could not accept connection" << endl;
        return -1;
    }
    return 0;
}
```


---

# SFML Networking Client

```cpp
#include <iostream>
#include <SFML/Network.hpp>

using namespace std;
using namespace sf;

int main(int argc, char **argv) {
    TcpSocket socket;
    Socket::Status status = socket.connect("127.0.0.1", 5000);
    if (status != Socket::Done) {
        cout << "Error - could not connect to server" << endl;
        return -1;
    }
    return 0;
}
```

---

## Summary


---

# Summary

- We have defined the issue of networking and discussed how we solve game networking issues.
- We looked at networking layers and protocols.
- We looked at network sockets.
- We looked at the limitations of networking by analysing the common metrics.
- We looked at how we overcome these limitations in games by looking at communicating patterns and UDP.
- We then discussed what data to send and how by discussing protocols.
- And finally we looked at how we use SFML for networking by presenting a client and server example.
- This is all you need to get started with networking for games using SFML. The lab will provide a concrete application.
