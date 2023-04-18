#pragma once

#include <deque>
#include <string>
#include <vector>

namespace communication_interfaces {

/**
 * @brief The ByteArray wraps a dynamic array of bytes (i.e. char).
 * @details The ByteArray provides convenient methods for loading and unloading data types to and from the ByteArray.
 * The class actas as an interface definition to raw data (in case the underlying structure of the raw data changes).
 * It's intended use is for socket communication.
 * By default, data is appended/removed from the end of the array using the load/unload methods.
 */
class ByteArray {
public:
  /**
   * @brief Default constructor to create an empty ByteArray
   */
  ByteArray();

  ~ByteArray() = default;

  /**
   * @brief Clear the buffer
   */
  void reset();

  /**
   * @brief Load a void* (treated as char*) into the buffer
   * @param value The value to load
   * @param size The number of bytes to load
   * @return True on success, false otherwise
   */
  bool load(const void* value, const std::size_t& size);

  /**
   * @brief Unload a void* (treated as char*) from the buffer
   * @param value The value to unload
   * @param size The number of bytes to unload
   * @return True on success, false otherwise
   */
  bool unload(void* value, const std::size_t& size);

  /**
   * @brief Copy the content of the buffer to a std::string
   * @param value The string to copy to
   */
  void copy_to(std::string& value) const;

  /**
   * @brief Copy the content of the buffer to a std::vector of char
   * @param value The vector to copy into
   */
  void copy_to(std::vector<char>& value) const;

  /**
   * @brief Set the buffer content from a std::string
   * @param value The string to copy from
   */
  void copy_from(const std::string& value);

  /**
   * @brief Set the ByteArray buffer from a std::vector of char
   * @param value The vector to copy from
   */
  void copy_from(const std::vector<char>& value);

  /**
   * @brief Get the current size of the buffer
   */
  [[nodiscard]] unsigned int get_buffer_size() const;

  /**
   * @brief Get the maximal size of the buffer
   */
  [[nodiscard]] unsigned int get_max_buffer_size() const;

private:
  std::deque<char> buffer_;
};
}// namespace communication_interfaces