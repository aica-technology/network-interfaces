#include "communication_interfaces/sockets/ZMQPublisher.hpp"

namespace communication_interfaces::sockets {

using namespace state_representation;

ZMQPublisher::ZMQPublisher(const std::shared_ptr<zmq::context_t>& context) : ZMQSocket(context) {}

ZMQPublisher::ZMQPublisher(const ParameterInterfaceList& parameters, const std::shared_ptr<zmq::context_t>& context) :
    ZMQSocket(parameters, context) {}

void ZMQPublisher::open() {
  this->socket_ = std::make_shared<zmq::socket_t>(*this->context_, ZMQ_PUB);
  this->open_socket();
}

bool ZMQPublisher::receive_bytes(ByteArray&) {
  throw std::runtime_error("Receive not available for socket of type ZMQPublisher");
}
} // namespace communication_interfaces::sockets
