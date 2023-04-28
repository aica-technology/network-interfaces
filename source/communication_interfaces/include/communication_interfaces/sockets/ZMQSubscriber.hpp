#pragma once

#include "communication_interfaces/sockets/ZMQSocket.hpp"

namespace communication_interfaces::sockets {

class ZMQSubscriber : public ZMQSocket {
public:
  explicit ZMQSubscriber(const std::shared_ptr<zmq::context_t>& context);

  explicit ZMQSubscriber(
      const state_representation::ParameterInterfaceList& parameters, const std::shared_ptr<zmq::context_t>& context
  );

  void open() override;

  bool send_bytes(const ByteArray& buffer) override;
};
} // namespace communication_interfaces::sockets
