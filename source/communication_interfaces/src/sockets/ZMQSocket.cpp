#include "communication_interfaces/sockets/ZMQSocket.hpp"

#include "communication_interfaces/exceptions/SocketConfigurationException.hpp"

namespace communication_interfaces::sockets {

ZMQSocket::ZMQSocket(ZMQSocketConfiguration configuration) : config_(std::move(configuration)) {}

ZMQSocket::~ZMQSocket() {
  ZMQSocket::close();
}

void ZMQSocket::open_socket() {
  try {
    auto address = "tcp://" + this->config_.ip_address + ":" + this->config_.port;
    if (this->config_.bind) {
      this->socket_->bind(address);
    } else {
      this->socket_->connect(address);
    }
  } catch (const std::exception& ex) {
    throw exceptions::SocketConfigurationException("Socket configuration failed: " + std::string(ex.what()));
  }
}

bool ZMQSocket::receive_bytes(ByteArray& buffer) {
  zmq::recv_flags recv_flag = this->config_.wait ? zmq::recv_flags::none : zmq::recv_flags::dontwait;
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
  zmq::send_flags send_flags = this->config_.wait ? zmq::send_flags::none : zmq::send_flags::dontwait;
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

void ZMQSocket::close() {
  if (this->socket_ != nullptr && this->socket_->connected()) {
    this->socket_->close();
  }
}
} // namespace communication_interfaces::sockets
