#include "communication_interfaces/ByteArray.hpp"

namespace communication_interfaces {

ByteArray::ByteArray() {
  this->reset();
}

void ByteArray::reset() {
  this->buffer_.clear();
}

bool ByteArray::load(const void* value, const size_t& size) {
  if (value == nullptr) {
    return false;
  }
  if (this->get_buffer_size() + size > this->get_max_buffer_size()) {
    return false;
  }
  try {
    char* byte_ptr = (char*)value;
    this->buffer_.insert(this->buffer_.end(), byte_ptr, byte_ptr + size);
    return true;
  } catch (const std::exception&) {
    return false;
  }
}

bool ByteArray::unload(void* value, const std::size_t& size) {
  if (value == nullptr) {
    return false;
  }
  if (size > this->get_buffer_size()) {
    return false;
  }
  try {
    std::copy(this->buffer_.end() - static_cast<long>(size), this->buffer_.end(), (char*)value);
    this->buffer_.erase(this->buffer_.end() - static_cast<long>(size), this->buffer_.end());
    return true;
  } catch (const std::exception&) {
    return false;
  }
}

void ByteArray::copy_to(std::vector<char>& value) const {
  value.assign(this->buffer_.begin(), this->buffer_.end());
}

void ByteArray::copy_to(std::string& value) const {
  std::vector<char> tmp;
  this->copy_to(tmp);
  value = std::string(tmp.data(), tmp.size());
}

void ByteArray::copy_from(const std::vector<char>& value) {
  this->reset();
  this->load(value.data(), value.size());
}

void ByteArray::copy_from(const std::string& value) {
  this->reset();
  this->load(value.c_str(), value.size());
}

unsigned int ByteArray::get_buffer_size() const {
  return this->buffer_.size();
}

unsigned int ByteArray::get_max_buffer_size() const {
  return this->buffer_.max_size();
}
}// namespace communication_interfaces
