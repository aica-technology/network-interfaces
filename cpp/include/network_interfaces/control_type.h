#pragma once

#include <cstdint>

namespace network_interfaces {

/**
 * @enum control_type_t
 * @brief An enumeration of the possible control types.
 */
enum class control_type_t : std::uint8_t {
  UNDEFINED = 0, POSITION = 1, VELOCITY = 2, ACCELERATION = 3, EFFORT = 4,
};
}// namespace network_interfaces