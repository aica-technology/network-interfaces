#pragma once

#include <cstdint>
#include <ostream>

namespace network_interfaces {

/**
 * @enum control_type_t
 * @brief An enumeration of the possible control types.
 */
enum class control_type_t : std::uint8_t {
  UNDEFINED = 0,
  JOINT_POSITION = 1,
  JOINT_VELOCITY = 2,
  JOINT_ACCELERATION = 3,
  JOINT_EFFORT = 4,
  CARTESIAN_POSE = 5,
  CARTESIAN_TWIST = 6,
  CARTESIAN_ACCELERATION = 7,
  CARTESIAN_WRENCH = 8,
  END = 9,
};

inline std::ostream& operator<<(std::ostream& os, control_type_t control_type) {
  switch (control_type) {
    case control_type_t::UNDEFINED:
      os << "UNDEFINED";
      break;
    case control_type_t::JOINT_POSITION:
      os << "POSITION";
      break;
    case control_type_t::JOINT_VELOCITY:
      os << "VELOCITY";
      break;
    case control_type_t::JOINT_ACCELERATION:
      os << "ACCELERATION";
      break;
    case control_type_t::JOINT_EFFORT:
      os << "EFFORT";
      break;
    case control_type_t::CARTESIAN_POSE:
      os << "POSITION";
      break;
    case control_type_t::CARTESIAN_TWIST:
      os << "VELOCITY";
      break;
    case control_type_t::CARTESIAN_ACCELERATION:
      os << "ACCELERATION";
      break;
    case control_type_t::CARTESIAN_WRENCH:
      os << "EFFORT";
      break;
    default:
      os << "UNKNOWN CONTROL TYPE";
      break;
  }
  return os;
}

}// namespace network_interfaces