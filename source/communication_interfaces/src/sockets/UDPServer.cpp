#include "communication_interfaces/sockets/UDPServer.hpp"

namespace communication_interfaces::sockets {

UDPServer::UDPServer() : UDPSocket(), client_address_() {}

UDPServer::UDPServer(const std::list<std::shared_ptr<state_representation::ParameterInterface>>& parameters) :
    UDPSocket(parameters), client_address_() {}

void UDPServer::open() {
  this->open_socket(true);
}

bool UDPServer::receive_bytes(ByteArray& buffer) {
  return this->recvfrom(this->client_address_, buffer);
}

bool UDPServer::send_bytes(const ByteArray& buffer) {
  return this->sendto(this->client_address_, buffer);
}
} // namespace communication_interfaces::sockets
