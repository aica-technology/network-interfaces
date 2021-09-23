#pragma once

#include <exception>

namespace network_interfaces::zmq::exceptions {
class InvalidControlTypeVectorException : public std::runtime_error {
public:
  explicit InvalidControlTypeVectorException(const std::string& msg) : runtime_error(msg) {};
};
}// namespace network_interfaces::zmq::exceptions