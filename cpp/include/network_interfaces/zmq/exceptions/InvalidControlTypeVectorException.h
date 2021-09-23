#pragma once

#include <exception>

namespace network_interfaces::zmq::exceptions {

/*
 * @class InvalidControlTypeVectorException
 * @brief Exception that is thrown when the control_type vector of a CommandMessage is incorrect
 */
class InvalidControlTypeVectorException : public std::runtime_error {
public:
  explicit InvalidControlTypeVectorException(const std::string& msg) : runtime_error(msg) {};
};
}// namespace network_interfaces::zmq::exceptions