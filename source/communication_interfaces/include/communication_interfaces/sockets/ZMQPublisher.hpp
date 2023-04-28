#pragma once

#include "communication_interfaces/sockets/ZMQSocket.hpp"

namespace communication_interfaces::sockets {

class ZMQPublisher : public ZMQSocket {
public:
  explicit ZMQPublisher(const std::shared_ptr<zmq::context_t>& context);

  explicit ZMQPublisher(
      const state_representation::ParameterInterfaceList& parameters, const std::shared_ptr<zmq::context_t>& context
  );

  void open() override;

  bool receive_bytes(ByteArray& buffer) override;
};
} // namespace communication_interfaces::sockets
