#pragma once

#include "communication_interfaces/sockets/ZMQSocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @class ZMQPublisher
 * @brief Class to define a ZMQ publisher
 */
class ZMQPublisher : public ZMQSocket {
public:
  /**
   * @copydoc ZMQSocket::ZMQSocket(ZMQSocketConfiguration, const std::shared_ptr<zmq::context_t>&)
   */
  explicit ZMQPublisher(ZMQSocketConfiguration configuration, const std::shared_ptr<zmq::context_t>& context);

  /**
   * @copydoc ISocket::open()
   */
  void open() override;

  /**
   * @brief This method throws a runtime error as receiving is not available for a ZMQ publisher
   */
  bool receive_bytes(ByteArray& buffer) override;
};
} // namespace communication_interfaces::sockets
