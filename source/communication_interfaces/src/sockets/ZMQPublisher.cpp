#include "communication_interfaces/sockets/ZMQPublisher.hpp"

namespace communication_interfaces::sockets {

ZMQPublisher::ZMQPublisher(ZMQSocketConfiguration configuration) : ZMQSocket(std::move(configuration)) {}

void ZMQPublisher::open() {
  this->socket_ = std::make_shared<zmq::socket_t>(*this->config_.context, ZMQ_PUB);
  this->open_socket();
}

bool ZMQPublisher::receive_bytes(ByteArray&) {
  throw std::runtime_error("Receive not available for socket of type ZMQPublisher");
}
} // namespace communication_interfaces::sockets
