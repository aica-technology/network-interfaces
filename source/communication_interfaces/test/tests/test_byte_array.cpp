#include <gtest/gtest.h>

#include "communication_interfaces/ByteArray.hpp"

using namespace communication_interfaces;

TEST(TestByteArray, ByteArray) {
  std::string message = "test";
  auto data = ByteArray();
  data.copy_from(message);
  EXPECT_EQ(data.get_buffer_size(), message.size());
  std::string result;
  data.copy_to(result);
  EXPECT_STREQ(result.c_str(), message.c_str());

  data.load(message.c_str(), message.size());
  EXPECT_EQ(data.get_buffer_size(), 2 * message.size());
  data.copy_to(result);
  EXPECT_STREQ(result.c_str(), (message + message).c_str());
  char* raw_result = new char[message.size()];
  data.unload(raw_result, message.size());
  EXPECT_EQ(data.get_buffer_size(), message.size());

  data.reset();
  EXPECT_EQ(data.get_buffer_size(), 0);
}
