#pragma once

#include <arpa/inet.h>

#include "communication_interfaces/sockets/ISocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @brief Abstract class to define a generic UDP socket
 */
class UDPSocket : public ISocket {
public:
  /**
   * @brief Close the socket by calling UDPSocket::close()
   */
  ~UDPSocket() override;

  /**
   * @brief Close the socket
   */
  void close() override;

protected:
  /**
   * @brief Add the parameters with no or default value to the map
   */
  UDPSocket();

  /**
   * @brief Add and set the parameters with the parameters given as argument
   * @param parameters The list of parameters
   */
  explicit UDPSocket(const state_representation::ParameterInterfaceList& parameters);

  /**
   * @brief Perform steps to open the socket on the desired IP/port, set reuse and timeout options and bind if desired.
   * @param bind_socket If true, bind the socket (for a UDP server), no binding otherwise (for a UDP client)
   */
  void open_socket(bool bind_socket);

  /**
   * @brief Receive bytes from the socket
   * @param address The address from which to receive bytes
   * @param buffer The buffer to fill with the received bytes
   * @return True if bytes were received, false otherwise
   */
  [[nodiscard]] bool recvfrom(const sockaddr_in& address, ByteArray& buffer);

  /**
   * @brief Send bytes to the socket
   * @param address The address to send the bytes to
   * @param buffer The buffer with the bytes to send
   * @return True if bytes were sent, false otherwise
   */
  [[nodiscard]] bool sendto(const sockaddr_in& address, const ByteArray& buffer) const;

  sockaddr_in server_address_; ///< Address of the UDP server

private:
  /**
   * @brief Validate and set parameters
   * @param parameter A parameter interface pointer
   */
  void validate_and_set_parameter(const std::shared_ptr<state_representation::ParameterInterface>& parameter) override;

  std::shared_ptr<state_representation::Parameter<int>> buffer_size_; ///< Maximal size of buffer to receive

  int server_fd_; ///< File descriptor of the socket
  socklen_t addr_len_; ///< Length of the socket address
};
} // namespace communication_interfaces::sockets
