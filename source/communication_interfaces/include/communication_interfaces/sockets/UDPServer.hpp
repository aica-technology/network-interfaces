#pragma once

#include "communication_interfaces/sockets/UDPSocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @class UDPServer
 * @brief Class to define a UDP server
 */
class UDPServer : public UDPSocket {
public:
  /**
   * @copydoc UDPSocket::UDPSocket(UDPSocketConfiguration)
   */
  UDPServer(UDPSocketConfiguration configuration);

  /**
   * @copydoc ISocket::open()
   */
  void open() override;

  /**
   * @copydoc ISocket::receive_bytes(std::string&)
   */
  bool receive_bytes(std::string& buffer) override;

  /**
   * @copydoc ISocket::send_bytes(const std::string&)
   */
  bool send_bytes(const std::string& buffer) override;

private:
  sockaddr_in client_address_;
};
} // namespace communication_interfaces::sockets
