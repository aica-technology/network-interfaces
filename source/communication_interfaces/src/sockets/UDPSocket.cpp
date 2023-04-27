#include "communication_interfaces/sockets/UDPSocket.hpp"

#include <unistd.h>
#include <sys/socket.h>

#include "communication_interfaces/exceptions/SocketConfigurationException.hpp"

namespace communication_interfaces::sockets {

using namespace state_representation;

UDPSocket::UDPSocket() :
    server_address_(), buffer_size_(std::make_shared<Parameter<int>>("buffer_size")), server_fd_(), addr_len_() {
  this->parameters_.insert_or_assign("ip_address", std::make_shared<Parameter<std::string>>("ip_address"));
  this->parameters_.insert_or_assign("port", std::make_shared<Parameter<int>>("port"));
  this->parameters_.insert_or_assign("enable_reuse", std::make_shared<Parameter<bool>>("enable_reuse", false));
  this->parameters_.insert_or_assign(
      "timeout_duration_sec", std::make_shared<Parameter<double>>("timeout_duration_sec", 0.0));
  this->parameters_.insert(std::make_pair("buffer_size", this->buffer_size_));
}

UDPSocket::UDPSocket(const ParameterInterfaceList& parameters) : UDPSocket() {
  this->set_parameters(parameters);
}

UDPSocket::~UDPSocket() {
  UDPSocket::close();
}

void UDPSocket::open_socket(bool bind_socket) {
  if (this->buffer_size_->is_empty()) {
    throw exceptions::SocketConfigurationException("Parameter 'buffer_size' is empty.");
  }

  try {
    this->addr_len_ = sizeof(this->server_address_);
    this->server_address_.sin_family = AF_INET;
    this->server_address_.sin_addr.s_addr = inet_addr(this->get_parameter_value<std::string>("ip_address").c_str());
    this->server_address_.sin_port = htons(this->get_parameter_value<int>("port"));
  } catch (const std::exception& ex) {
    throw exceptions::SocketConfigurationException("Socket configuration failed: " + std::string(ex.what()));
  }

  this->server_fd_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (this->server_fd_ < 0) {
    throw exceptions::SocketConfigurationException("Opening socket failed");
  }
  if (this->get_parameter_value<bool>("enable_reuse")) {
    const int opt_reuse = 1;
    if (setsockopt(this->server_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt_reuse, sizeof(opt_reuse)) != 0) {
      throw exceptions::SocketConfigurationException("Setting socket option (enable reuse) failed");
    }
  }
  if (bind_socket) {
    if (bind(this->server_fd_, (sockaddr*) &(this->server_address_), sizeof(server_address_)) != 0) {
      throw exceptions::SocketConfigurationException("Binding socket failed.");
    }
  }

  auto timeout_duration_sec = this->get_parameter_value<double>("timeout_duration_sec");
  if (timeout_duration_sec > 0.0) {
    timeval timeout{};
    auto secs = std::floor(timeout_duration_sec);
    timeout.tv_sec = static_cast<long>(secs);
    timeout.tv_usec = static_cast<long>((timeout_duration_sec - secs) * 1e6);
    if (setsockopt(this->server_fd_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) != 0) {
      throw exceptions::SocketConfigurationException("Setting socket timeout failed.");
    }
  }
}

bool UDPSocket::recvfrom(sockaddr_in& address, ByteArray& buffer) {
  std::vector<char> local_buffer(this->buffer_size_->get_value());
  auto receive_length = ::recvfrom(
      this->server_fd_, local_buffer.data(), this->buffer_size_->get_value(), 0, (sockaddr*) &(address),
      &(this->addr_len_));
  if (receive_length < 0) {
    return false;
  }
  local_buffer.at(receive_length) = 0;
  buffer.reset();
  buffer.load(local_buffer.data(), receive_length);
  return true;
}

bool UDPSocket::sendto(const sockaddr_in& address, const ByteArray& buffer) const {
  std::vector<char> local_buffer;
  buffer.copy_to(local_buffer);
  int send_length = ::sendto(
      this->server_fd_, local_buffer.data(), local_buffer.size(), 0, (sockaddr*) &(address), this->addr_len_);
  return send_length == static_cast<int>(local_buffer.size());
}

void UDPSocket::validate_and_set_parameter(const std::shared_ptr<ParameterInterface>& parameter) {
  // TODO remove once this check is included in `assert_parameter_valid`
  if (this->parameters_.find(parameter->get_name()) == this->parameters_.end()) {
    throw state_representation::exceptions::InvalidParameterException(
        "Invalid parameter '" + parameter->get_name() + "' for class UDPSocket.");
  }
  this->assert_parameter_valid(parameter);
  if (parameter->is_empty()) {
    throw state_representation::exceptions::InvalidParameterException(
        "Parameter '" + parameter->get_name() + "' cannot be empty.");
  }
  if (parameter->get_name() == "timeout_duration_sec" && parameter->get_parameter_value<double>() < 0.0) {
    throw state_representation::exceptions::InvalidParameterException(
        "Parameter 'timeout_duration_sec' cannot be negative.");
  }
  if (parameter->get_name() == "buffer_size") {
    int value = parameter->get_parameter_value<int>();
    if (value < 0) {
      throw state_representation::exceptions::InvalidParameterException(
          "Parameter 'buffer_size' cannot be negative.");
    }
    this->parameters_.at(parameter->get_name())->set_parameter_value(value);
  } else {
    this->parameters_.insert_or_assign(parameter->get_name(), parameter);
  }
}

void UDPSocket::close() {
  if (this->server_fd_ >= 0) {
    ::close(this->server_fd_);
  }
}
} // namespace communication_interfaces::sockets
