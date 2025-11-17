---
title: "Lecture18"
keywords: Lecture
tags: [Lecture]
permalink:  lecture18.html
summary: lecture18
layout: presentation
presentationTheme: '/assets/revealJS/css/theme/napier.css' 
---
<section data-markdown data-separator="^\n---\n$" data-separator-vertical="^\n--\n$">
<textarea data-template>

# Lecture 18 - Networking
### SET09121 - Games Engineering

<br><br>
Babis Koniaris/Tobias Grubenmann
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

# The Seven Layer OSI Model

- Networking works on a layered model.
    - Application data at the top layer.
    - Electrical signals at the bottom layer.
- As programmers, we rarely need to consider below level 3.


![image](assets/images/osi_layers.jpg) <!-- .element width="60%"  -->

---

# Addressing and Switching

- Networking works like a mail system.
- We send data via packets (video file: lots of packets, text file: few packets)
- Each individual packet has an address (IP address) which routers and switches have to look at.
- Routers forward packets to different computer networks
- Switches forward packets to the correct device in a network
- Packets can be lost

---

# Transport Control Protocol - TCP

- TCP provides means of breaking down data into separate packets.
- Each data chunk is given a sequence number to allow the data to be reformed.
- TCP guarantees acknowledgement of sent data - this means we know that data has arrived if we don't get an error.
- TCP is the most common distributed application protocol because of its guarantees.
- However, it also introduces higher latency. When this is an issue, we use UDP.

---

## How do we do network programming?


---

# Client-server Model

- The most common application model in networking is the client-server.
- A client connects to a server machine via some form of universal addressing.
- The server can now communicate directly with that client.
- A server may have multiple clients that it is communicating with.


 ![image](assets/images/client-server.png) <!-- .element width="60%"  -->



---

# Sockets

- Applications communicate using sockets.
- A socket is just an encapsulation of the following:
    - Address: typically an IP address.
    - Protocol: for example TCP.
    - Port: allows an individual application to be addressed on the network.
- These are provided by layers 3-5 of the OSI model.
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
- Here, each client would update its move to the other clients.
- The game would wait until everyone has updated before moving onto the next move.
- It was slow as you can guess.


 ![image](assets/images/p2p.png)



---

# Client-side Prediction

- Nowadays games use prediction algorithms to counter latency
- Client handles input locally, and sends it to server
- After server game state update, clients gets update too
- If new state different from predicted state: smooth/interpolate


 ![image](assets/images/client-prediction.png)



---

# User Datagram Packet - UDP

- As we are now predicting movements and locations, we can occasionally lose information without much concern.
- Therefore, we do not need packet guarantees. TCP is no longer needed.
- UDP is an alternate protocol which is connectionless. We just send data to a location.
- The receiver will keep looking for new data packets as often as it can.
- Basically, we can improve performance considerably by not acknowledging data sends.


---

# TCP vs UDP Header

![image](assets/images/tcp_udp_header.jpg) <!-- .element width="60%"  -->


---

# Synchronous vs Asynchronous Polling

- We can also choose how we receive data from connections.
- Typically, we **wait** on a connection until data arrives. Really bad if done in main thread.
- Asynchronous socket communication means we don't wait: we just read otherwise we move on.
- This allows the game to continue on and we can check again later.

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

- Scenes/levels are typically the same for all clients, only starting parameters differ
- We only need to share what is different:
	- Player start position: ok
	- Positions of initial scene elements: **unnecessary**

---

# Scene Updates

- At every network "tick" you need to communicate data between the client and server.
- The communication must update the server with information the client has on scene updates.
- The communication must update the client with information the server has on scene updates.
- Every so often, the client and server must do a more complete update to normalise their information.
- Effectively, we are trying to keep the client and server as synchronised as possible without performing lockstep.


---

# Object Serialisation

- The process of converting a data object into a series of bytes
- Deserialise: opposite process
- Can be text (e.g. JSON, XML) or binary
- Several managed languages have good built-in serialisation because of their support for reflection.
- Reflection is the capability to inspect type information at runtime
- C++ has poor reflection support, and poor built-in serialisation capabilities


---

# Designing a Protocol

- Define a fixed size for the messages.
- Define a messages formats: 
    - Design your message types and the data that will go into the message
- Limit serialisation size
- This will allow simple messaging that can be easily managed
- Here is a one-size-fits-all message:

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

# Summary

- The throughput of messages needs to be smaller than the bandwidth
- You can't send at every update all the game informations.
- Define a communication protocol: the messages format.
- If TCP is to slow try to use UDP but you will have to handle manually networking errors.
- The most common and basic model is the Client/Server which is the one implemented in SFML. 


