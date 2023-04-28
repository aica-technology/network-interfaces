#include "communication_interfaces/sockets/ZMQSocket.hpp"

#include "communication_interfaces/exceptions/SocketConfigurationException.hpp"

namespace communication_interfaces::sockets {

using namespace state_representation;

ZMQSocket::ZMQSocket(const std::shared_ptr<zmq::context_t>& context) :
    context_(context), wait_(std::make_shared<Parameter<bool>>("wait", false)) {
  this->parameters_.insert_or_assign("ip_address", std::make_shared<Parameter<std::string>>("ip_address"));
  this->parameters_.insert_or_assign("port", std::make_shared<Parameter<int>>("port"));
  this->parameters_.insert_or_assign("bind_socket", std::make_shared<Parameter<bool>>("bind_socket"));
  this->parameters_.insert(std::make_pair("wait", this->wait_));
}

ZMQSocket::ZMQSocket(const ParameterInterfaceList& parameters, const std::shared_ptr<zmq::context_t>& context) :
    ZMQSocket(context) {
  this->set_parameters(parameters);
}

ZMQSocket::~ZMQSocket() {
  ZMQSocket::close();
}

void ZMQSocket::open_socket() {
  try {
    auto address = "tcp://" + this->get_parameter_value<std::string>("ip_address") + ":"
        + this->get_parameter_value<std::string>("port");
    if (this->get_parameter_value<bool>("bind_socket")) {
      this->socket_->bind(address);
    } else {
      this->socket_->connect(address);
    }
  } catch (const std::exception& ex) {
    throw exceptions::SocketConfigurationException("Socket configuration failed: " + std::string(ex.what()));
  }
}

bool ZMQSocket::receive_bytes(ByteArray& buffer) {
  zmq::recv_flags recv_flag = this->wait_->get_value() ? zmq::recv_flags::none : zmq::recv_flags::dontwait;
  zmq::message_t message;
  try {
    auto received = this->socket_->recv(message, recv_flag);
    if (received.has_value()) {
      buffer.reset();
      buffer.load(message.data(), message.size());
    }
    return received.has_value();
  } catch (const zmq::error_t&) {
    return false;
  }
}

bool ZMQSocket::send_bytes(const ByteArray& buffer) {
  zmq::send_flags send_flags = this->wait_->get_value() ? zmq::send_flags::none : zmq::send_flags::dontwait;
  std::vector<char> local_buffer;
  buffer.copy_to(local_buffer);
  zmq::message_t msg(local_buffer.begin(), local_buffer.end());
  try {
    auto sent = this->socket_->send(msg, send_flags);
    return sent.has_value();
  } catch (const zmq::error_t&) {
    return false;
  }
}

void ZMQSocket::validate_and_set_parameter(const std::shared_ptr<ParameterInterface>& parameter) {
  this->assert_parameter_valid(parameter);
  if (parameter->is_empty()) {
    throw state_representation::exceptions::InvalidParameterException(
        "Parameter '" + parameter->get_name() + "' cannot be empty.");
  }
  if (parameter->get_name() == "wait") {
    this->parameters_.at(parameter->get_name())->set_parameter_value(parameter->get_parameter_value<bool>());
  } else {
    this->parameters_.insert_or_assign(parameter->get_name(), parameter);
  }
}

void ZMQSocket::close() {
  if (this->socket_ != nullptr && this->socket_->connected()) {
    this->socket_->close();
  }
}
} // namespace communication_interfaces::sockets
