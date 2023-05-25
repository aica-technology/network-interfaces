#include "communication_interfaces/sockets/ZMQSubscriber.hpp"

namespace communication_interfaces::sockets {

ZMQSubscriber::ZMQSubscriber(ZMQSocketConfiguration configuration) : ZMQSocket(std::move(configuration)) {}

void ZMQSubscriber::open() {
  this->socket_ = std::make_shared<zmq::socket_t>(*this->config_.context, ZMQ_SUB);
  this->open_socket();
  this->socket_->set(zmq::sockopt::conflate, 1);
  this->socket_->set(zmq::sockopt::subscribe, "");
}

bool ZMQSubscriber::send_bytes(const ByteArray&) {
  throw std::runtime_error("Send not available for socket of type ZMQSubscriber");
}
} // namespace communication_interfaces::sockets
