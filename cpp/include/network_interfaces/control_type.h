#pragma once

#include <cstdint>

namespace network_interfaces {

/**
 * @enum control_type_t
 * @brief An enumeration of the possible control types.
 */
enum class control_type_t : std::uint8_t {
  UNDEFINED = 0, POSITION = 1, VELOCITY = 2, ACCELERATION = 3, EFFORT = 4, END = 5,
};

inline std::ostream& operator<<(std::ostream& os, control_type_t control_type) {
  switch (control_type) {
    case control_type_t::UNDEFINED:
      os << "UNDEFINED";
      break;
    case control_type_t::POSITION:
      os << "POSITION";
      break;
    case control_type_t::VELOCITY:
      os << "VELOCITY";
      break;
    case control_type_t::ACCELERATION:
      os << "ACCELERATION";
      break;
    case control_type_t::EFFORT:
      os << "EFFORT";
      break;
    default:
      os << "UNKNOWN CONTROL TYPE";
      break;
  }
  return os;
}

}// namespace network_interfaces