#pragma once

#include "communication_interfaces/sockets/UDPSocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @brief Class to define a UDP client
 */
class UDPClient : public UDPSocket {
public:
  /**
   * @copydoc UDPSocket::UDPSocket()
   */
  UDPClient();

  /**
   * @copydoc UDPSocket::UDPSocket(const state_representation::ParameterInterfaceList&)
   */
  explicit UDPClient(const state_representation::ParameterInterfaceList& parameters);

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
};
} // namespace communication_interfaces::sockets
