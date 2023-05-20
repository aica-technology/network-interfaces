#pragma once

#include <zmq.hpp>

#include "communication_interfaces/sockets/ISocket.hpp"

namespace communication_interfaces::sockets {

/**
 * @struct ZMQSocketConfiguration
 * @brief Configuration parameters for a ZMQ socket
 */
struct ZMQSocketConfiguration {
  ZMQSocketConfiguration(std::string ip_address, std::string port, bool bind_socket, bool wait = false) :
      ip_address(std::move(ip_address)), port(std::move(port)), bind_socket(bind_socket), wait(wait) {}

  std::string ip_address;
  std::string port;
  bool bind_socket;
  bool wait;
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
   * @copydoc ISocket::receive_bytes(ByteArray&)
   */
  bool receive_bytes(ByteArray& buffer) override;

  /**
   * @copydoc ISocket::send_bytes(ByteArray&)
   */
  bool send_bytes(const ByteArray& buffer) override;

protected:
    /**
     * @brief Constructor taking the configuration struct
     */
  /**
   * @brief Constructor taking the configuration struct
   * @param The configuration struct
   * @param context The ZMQ context to be used for the sockets
   * (it's recommended to only use one context per application)
   */
  ZMQSocket(ZMQSocketConfiguration configuration, const std::shared_ptr<zmq::context_t>& context);

  /**
   * @brief Bind or connect the socket on the desired IP/port
   */
  void open_socket();

  ZMQSocketConfiguration config_; ///< Socket configuration struct
  std::shared_ptr<zmq::context_t> context_; ///< ZMQ context
  std::shared_ptr<zmq::socket_t> socket_; ///< ZMQ socket
};
} // namespace communication_interfaces::sockets
