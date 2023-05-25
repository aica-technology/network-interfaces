#include "communication_interfaces/sockets/UDPServer.hpp"

namespace communication_interfaces::sockets {

UDPServer::UDPServer(UDPSocketConfiguration configuration) : UDPSocket(std::move(configuration)) {}

void UDPServer::open() {
  this->open_socket(true);
}

bool UDPServer::receive_bytes(std::string& buffer) {
  return this->recvfrom(this->client_address_, buffer);
}

bool UDPServer::send_bytes(const std::string& buffer) {
  return this->sendto(this->client_address_, buffer);
}
} // namespace communication_interfaces::sockets
