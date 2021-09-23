#pragma once

#include <exception>

namespace network_interfaces::zmq::exceptions {

/*
 * @class UnknownControlTypeException
 * @brief Exception that is thrown when a control_type of a CommandMessage is unknown
 */
class UnknownControlTypeException : public std::runtime_error {
public:
  explicit UnknownControlTypeException(const std::string& msg) : runtime_error(msg) {};
};
}// namespace network_interfaces::zmq::exceptions