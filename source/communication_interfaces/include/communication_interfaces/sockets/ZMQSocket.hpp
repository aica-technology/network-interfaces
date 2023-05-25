#pragma once

#include <zmq.hpp>

#include "communication_interfaces/sockets/ISocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @struct ZMQSocketConfiguration
 * @brief Configuration parameters for a ZMQ socket
 */
struct ZMQSocketConfiguration {
  std::shared_ptr<zmq::context_t> context;
  std::string ip_address;
  std::string port;
  bool bind = true;
  bool wait = true;
};

/**
 * @class ZMQSocket
 * @brief Abstract class to define a generic ZMQ socket
 */
class ZMQSocket : public ISocket {
public:
  /**
   * @brief Close the socket by calling ZMQSocket::close()
   */
  ~ZMQSocket() override;

  /**
   * @brief Close the socket
   */
  void close() override;

  /**
   * @copydoc ISocket::receive_bytes(std::string&)
   */
  bool receive_bytes(std::string& buffer) override;

  /**
   * @copydoc ISocket::send_bytes(const std::string&)
   */
  bool send_bytes(const std::string& buffer) override;

protected:
  /**
   * @brief Constructor taking the configuration struct
   * @param The configuration struct
   */
  ZMQSocket(ZMQSocketConfiguration configuration);

  /**
   * @brief Bind or connect the socket on the desired IP/port
   */
  void open_socket();

  ZMQSocketConfiguration config_; ///< Socket configuration struct
  std::shared_ptr<zmq::socket_t> socket_; ///< ZMQ socket
};
} // namespace communication_interfaces::sockets
