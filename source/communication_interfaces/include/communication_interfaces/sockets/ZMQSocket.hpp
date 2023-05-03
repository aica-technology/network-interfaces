#pragma once

#include <zmq.hpp>

#include "communication_interfaces/sockets/ISocket.hpp"

namespace communication_interfaces::sockets {

/**
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
    * @brief Add the parameters with no or default value to the map
    * @param context The ZMQ context to be used for the sockets
    * (it's recommended to only use one context per application)
    */
  explicit ZMQSocket(const std::shared_ptr<zmq::context_t>& context);

  /**
   * @brief Add and set the parameters with the parameters given as argument
   * @param parameters The list of parameters
   * @param context The ZMQ context to be used for the sockets
    * (it's recommended to only use one context per application)
   */
  ZMQSocket(
      const state_representation::ParameterInterfaceList& parameters, const std::shared_ptr<zmq::context_t>& context
  );

  /**
   * @brief Bind or connect the socket on the desired IP/port
   */
  void open_socket();

  std::shared_ptr<zmq::context_t> context_; ///< ZMQ context
  std::shared_ptr<zmq::socket_t> socket_; ///< ZMQ socket

private:
  /**
   * @brief Validate and set parameters
   * @param parameter A parameter interface pointer
   */
  void validate_and_set_parameter(const std::shared_ptr<state_representation::ParameterInterface>& parameter) final;

  std::shared_ptr<state_representation::Parameter<bool>> wait_; ///< If false, send and receive are blocking
};
} // namespace communication_interfaces::sockets
