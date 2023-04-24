#pragma once

#include <state_representation/parameters/ParameterMap.hpp>

#include "communication_interfaces/ByteArray.hpp"

namespace communication_interfaces::sockets {

/**
 * @brief Interface class to define functions required for simple socket communication
 */
class ISocket : public state_representation::ParameterMap {
public:
  /**
   * @brief Default constructor
   */
  ISocket() = default;

  /**
   * @brief Default destructor
   */
  virtual ~ISocket() = default;

  /**
   * @brief Perform configuration steps to open the socket for communication
   * @throws SocketConfigurationException if opening fails
   */
  virtual void open() = 0;

  /**
   * @brief Receive bytes from the socket
   * @param buffer The buffer to fill with the received bytes
   * @return True if bytes were received, false otherwise
   */
  virtual bool receive_bytes(ByteArray& buffer) = 0;

  /**
   * @brief Send bytes to the socket
   * @param buffer The buffer with the bytes to send
   * @return True if bytes were sent, false otherwise
   */
  virtual bool send_bytes(const ByteArray& buffer) = 0;

  /**
   * @brief Perform steps to disconnect and close the socket communication
   */
  virtual void close() {}
};
}// namespace communication_interfaces::sockets
