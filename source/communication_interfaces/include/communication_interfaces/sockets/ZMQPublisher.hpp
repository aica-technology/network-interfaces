#pragma once

#include "communication_interfaces/sockets/ZMQSocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @brief Class to define a ZMQ publisher
 */
class ZMQPublisher : public ZMQSocket {
public:
  /**
   * @copydoc ZMQSocket::ZMQSocket(const std::shared_ptr<zmq::context_t>&)
   */
  explicit ZMQPublisher(const std::shared_ptr<zmq::context_t>& context);

  /**
   * @copydoc ZMQSocket::ZMQSocket(const state_representation::ParameterInterfaceList&, const std::shared_ptr<zmq::context_t>&)
   */
  explicit ZMQPublisher(
      const state_representation::ParameterInterfaceList& parameters, const std::shared_ptr<zmq::context_t>& context
  );

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
