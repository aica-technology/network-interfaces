#pragma once

#include "communication_interfaces/sockets/UDPSocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @class UDPClient
 * @brief Class to define a UDP client
 */
class UDPClient : public UDPSocket {
public:
  /**
   * @copydoc UDPSocket::UDPSocket(UDPSocketConfiguration)
   */
  UDPClient(UDPSocketConfiguration configuration);

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
};
} // namespace communication_interfaces::sockets
