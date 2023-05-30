#pragma once

#include "communication_interfaces/sockets/TCPSocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @struct TCPServerConfiguration
 * @brief Configuration parameters for a TCP server
 */
struct TCPServerConfiguration {
  int port;
  int buffer_size;
  bool enable_reuse;
};

class TCPServer : public TCPSocket {
public:
  /**
   * @brief Constructor taking the configuration struct
   * @param The configuration struct
   */
  explicit TCPServer(TCPServerConfiguration configuration);

  /**
   * @brief Close the sockets by calling TCPServer::close()
   */
  ~TCPServer() override;

  /**
   * @copydoc ISocket::open()
   * @details Wait for connection requests from clients and accept new connections. This method blocks until a
   * connection is established
   */
  void open() override;

  /**
   * @brief Close the sockets
   */
  void close() override;

private:
  TCPServerConfiguration config_; ///< Socket configuration struct
  int server_fd_; ///< File descriptor of the connected server socket
};
} // namespace communication_interfaces::sockets
