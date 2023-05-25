#pragma once

#include <arpa/inet.h>

#include "communication_interfaces/sockets/ISocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @brief Abstract class to define a generic TCP socket
 * @details TCP is a connection-based communication protocol. Hence, TCP sockets need to implement an additional
 * interface method `connect()`.
 */
class TCPSocket : public ISocket {
public:
  /**
   * @brief Close the socket by calling TCPSocket::close()
   */
  ~TCPSocket() override;

  /**
   * @brief Connect the TCP socket to its counterpart (client / server)
   * @details This method should be called after `open()` and may be blocking (for the case of a server)
   */
  virtual void connect() = 0;

  /**
   * @brief Getter of the connected attribute
   * @return True if socket is connected
   */
  [[nodiscard]] bool is_connected() const;

  /**
   * @copydoc ISocket::receive_bytes(std::string&)
   */
  bool receive_bytes(std::string& buffer) override;

  /**
   * @copydoc ISocket::receive_bytes(std::string&)
   */
  bool send_bytes(const std::string& buffer) override;

  /**
   * @brief Close the socket
   */
  void close() override;

protected:
  explicit TCPSocket(int buffer_size);

  sockaddr_in server_address_; ///< Address of the TCP server
  int socket_fd_; ///< File descriptor of the socket
  bool connected_; ///< Bool to indicate connection status
  int buffer_size_; ///< Buffer size
};
} // namespace communication_interfaces::sockets
