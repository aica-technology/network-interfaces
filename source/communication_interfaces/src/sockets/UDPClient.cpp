#include "communication_interfaces/sockets/UDPClient.hpp"

namespace communication_interfaces::sockets {

UDPClient::UDPClient(UDPSocketConfiguration configuration) : UDPSocket(std::move(configuration)) {}

void UDPClient::open() {
  this->open_socket(false);
}

bool UDPClient::receive_bytes(ByteArray& buffer) {
  return this->recvfrom(this->server_address_, buffer);
}

bool UDPClient::send_bytes(const ByteArray& buffer) {
  return this->sendto(this->server_address_, buffer);
}
} // namespace communication_interfaces::sockets
