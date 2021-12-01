# PacketGen
C++ Serialization Code Generator in Python.

Generates Classes capable of Serialization/Deserialization using `std::istream&` and `std::ostream&`, see `membuf.h` to stream directly into a char array.

## Usage

Simply run with `python packet-gen.py <input file name> <client output file name> <server output file name>`

### Example Input file content

For a full example, see [example-definition.txt](https://github.com/BrunoC-L/PacketGen/blob/main/example-definition.txt). So far, the only base cases supported are `primitives` defined in [constants.py](https://github.com/BrunoC-L/PacketGen/blob/main/src/constants.py.txt) and arrays (transpiled into vectors).

```
Players
	players
		Player[]

Player
	id
		u16
	name
		str
	friends
        	str[]
	pos
		PlayerPosition
...
```

## What does it do exactly?

The current version generates 2 headers by default since my use case is for serialization & deserialization of packets sent over TCP, see [this repo](https://github.com/BrunoC-L) for an integrated prototype with `boost::asio::ip::tcp`, a server and multiple client threads. The clients and server get different files because some packets are read only or write only from their point of view. Using `C2S` or `S2C` in the packet names to signify Client to Server or Server to Client, one can further specify how the classes will be generated.

A typed subscription system through a dispatcher is also generated, when deserializing, you always need to know what you are reading, so there are built-in functionalities for writing the length and type of each packet into the stream. The packet length is only to be trusted by the creator to know how many bytes to ask the socket to send. When receiving a packet, first read the length as the first 2 bytes of the message, then read <length> bytes as the message and call `Dispatcher::dispatch(...)`. The server and client differ on this point, the server requires an extra parameter for dispatching which is which `Sender&` is responsible for sending this packet. It is highly recommended checking out the repo mentionned above to understand how it integrates with the application, how to send and receive typed objects.

## Possible extentions

- Generating a single header with the client version of the dispatcher for non-client-server application use cases.
- Calculating the size of a packet before streaming it, allowing for allocating precisely the required array size.
