#pragma once

#include <zmq.hpp>

#include "communication_interfaces/sockets/ISocket.hpp"

namespace communication_interfaces::sockets {

class ZMQSocket : public ISocket {
public:
  ~ZMQSocket() override;

  void close() override;

  bool receive_bytes(ByteArray& buffer) override;

  bool send_bytes(const ByteArray& buffer) override;

protected:
  explicit ZMQSocket(const std::shared_ptr<zmq::context_t>& context);

  ZMQSocket(
      const state_representation::ParameterInterfaceList& parameters, const std::shared_ptr<zmq::context_t>& context
  );

  void open_socket();

  std::shared_ptr<zmq::context_t> context_;
  std::shared_ptr<zmq::socket_t> socket_;

private:
  /**
   * @brief Validate and set parameters
   * @param parameter A parameter interface pointer
   */
  void validate_and_set_parameter(const std::shared_ptr<state_representation::ParameterInterface>& parameter) final;

  std::shared_ptr<state_representation::Parameter<bool>> wait_;
};
} // namespace communication_interfaces::sockets
