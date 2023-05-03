#pragma once

#include "communication_interfaces/sockets/ZMQSocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @brief Class to define a ZMQ subscriber
 */
class ZMQSubscriber : public ZMQSocket {
public:
  /**
   * @copydoc ZMQSocket::ZMQSocket(const std::shared_ptr<zmq::context_t>&)
   */
  explicit ZMQSubscriber(const std::shared_ptr<zmq::context_t>& context);

  /**
   * @copydoc ZMQSocket::ZMQSocket(const state_representation::ParameterInterfaceList&, const std::shared_ptr<zmq::context_t>&)
   */
  explicit ZMQSubscriber(
      const state_representation::ParameterInterfaceList& parameters, const std::shared_ptr<zmq::context_t>& context
  );

  /**
   * @copydoc ISocket::open()
   */
  void open() override;

  /**
   * @brief This method throws a runtime error as sending is not available for a ZMQ publisher
   */
  bool send_bytes(const ByteArray& buffer) override;
};
} // namespace communication_interfaces::sockets
