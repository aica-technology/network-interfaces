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
   * @copydoc ISocket::receive_bytes(ByteArray&)
   */
  bool receive_bytes(ByteArray& buffer) override;

  /**
   * @copydoc ISocket::send_bytes(const ByteArray&)
   */
  bool send_bytes(const ByteArray& buffer) override;

private:
  sockaddr_in client_address_;
};
} // namespace communication_interfaces::sockets
