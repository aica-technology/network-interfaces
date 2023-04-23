## Socket Interface

The `ISocket` class is an interface for simple socket communication, defining functions for opening a socket,
sending and receiving bytes, and closing the socket connection.

This interface extends from `state_representation::ParameterMap`, which allows for adding and retrieving parameters for
the socket connection. Further, the interface utilizes the `ByteArray` class to conveniently send and receive data in
the form of a dynamic array of bytes.

The `ISocket` class defines an `open()` method to perform configuration steps to open the socket for communication.
If opening the socket fails, an exception is thrown. The `close()` method is also provided to perform steps to disconnect
and close the socket communication.

The functions `receive_bytes(ByteArray&)` and `send_bytes(const ByteArray&)` perform the read and write logic of the socket
respectively.

### Implementing a derived socket class

To use this class, create a subclass that inherits from it and implement its pure virtual functions. The pure virtual
functions are `open()`, `receive_bytes(ByteArray&)`, and `send_bytes(const ByteArray&)`.

The `close()` function can optionally be overridden to perform steps to disconnect and close the socket communication.
If a derived class defines any cleanup behavior in `close()`, it should also be invoked statically and explicitly
in the destructor of the derived class.

An example is given below.

```c++
// DerivedSocket.hpp
class DerivedSocket : ISocket {
public:
  ~DerivedSocket() override;
  
  void open() override;
  
  bool receive_bytes(ByteArray& buffer) override;

  bool send_bytes(const ByteArray& buffer) override;
  
  void close() override;
}
```

```c++
// DerivedSocket.cpp
DerivedSocket::~DerivedSocket() {
  DerivedSocket::close();
}

void DerivedSocket::open() {
  // Configure and open the socket
}

bool DerivedSocket::receive_bytes(ByteArray& buffer) {
  // Read the contents of the socket into the buffer and return true on success. Otherwise, return false.
  return true;
}

bool DerivedSocket::send_bytes(const ByteArray& buffer) {
  // Write the contents of the buffer onto the socket and return true on success. Otherwise, return false.
  return true;
}

void DerivedSocket::close() {
  // Perform clean-up steps here
}
```

## Byte Array

The `ByteArray` class is a container for a dynamic array of bytes (i.e. `char`). It provides methods for loading and
unloading data types to and from the ByteArray. The class acts as an interface definition to raw data (in case the
underlying structure of the raw data changes). Its intended use is for socket communication.

To use this class, create a `ByteArray` object and use its member functions to load and unload data to and from the
buffer. The default behavior is to append/remove data from the end of the buffer. The class provides methods to copy the
buffer to and from a std::string or a std::vector<char>.
