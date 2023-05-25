## Socket Interface

The `ISocket` class is an interface for simple socket communication, defining functions for opening a socket,
sending and receiving bytes, and closing the socket connection.

The `ISocket` class defines an `open()` method to perform configuration steps to open the socket for communication.
If opening the socket fails, an exception is thrown. The `close()` method is also provided to perform steps to disconnect
and close the socket communication.

The functions `receive_bytes(std::string&)` and `send_bytes(const std::string&)` perform the read and write logic of the socket
respectively.

### Implementing a derived socket class

To use this class, create a subclass that inherits from it and implement its pure virtual functions. The pure virtual
functions are `open()`, `receive_bytes(std::string&)`, and `send_bytes(const std::string&)`.

Configuration parameters should be passed with a configuration struct, resulting in a single argument constructor.

The `close()` function can optionally be overridden to perform steps to disconnect and close the socket communication.
If a derived class defines any cleanup behavior in `close()`, it should also be invoked statically and explicitly
in the destructor of the derived class.

An example is given below.

```c++
// DerivedSocket.hpp

struct DerivedSocketConfig {
  int param1;
  double param2;
};

class DerivedSocket : ISocket {
public:
  DerivedSocket(DerivedSocketConfig configuration);
  
  ~DerivedSocket() override;
  
  void open() override;
  
  bool receive_bytes(std::string& buffer) override;

  bool send_bytes(const std::string& buffer) override;
  
  void close() override;
}
```

```c++
// DerivedSocket.cpp
DerivedSocket::DerivedSocket(DerivedSocketConfig configuraiton) {
  // save configuration parameters for later use
}

DerivedSocket::~DerivedSocket() {
  DerivedSocket::close();
}

void DerivedSocket::open() {
  // Configure and open the socket
}

bool DerivedSocket::receive_bytes(std::string& buffer) {
  // Read the contents of the socket into the buffer and return true on success. Otherwise, return false.
  return true;
}

bool DerivedSocket::send_bytes(const std::string& buffer) {
  // Write the contents of the buffer onto the socket and return true on success. Otherwise, return false.
  return true;
}

void DerivedSocket::close() {
  // Perform clean-up steps here
}
```
