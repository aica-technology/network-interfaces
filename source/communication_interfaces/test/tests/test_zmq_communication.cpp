#include <gtest/gtest.h>
#include <thread>

#include "communication_interfaces/sockets/ZMQPublisher.hpp"
#include "communication_interfaces/sockets/ZMQSubscriber.hpp"

using namespace communication_interfaces;
using namespace std::chrono_literals;

class TestZMQSockets : public ::testing::Test {
public:
  TestZMQSockets() {
    auto context = std::make_shared<zmq::context_t>(1);
    config_ = {context, "127.0.0.1", "4000"};
  }

  sockets::ZMQSocketConfiguration config_;
};

TEST_F(TestZMQSockets, SendReceive) {
  const std::string send_string = "Hello world!";

  sockets::ZMQPublisher publisher(this->config_);
  this->config_.bind = false;
  sockets::ZMQSubscriber subscriber(this->config_);

  publisher.open();
  subscriber.open();

  ByteArray message;
  message.copy_from(send_string);
  for (int i = 0; i < 5; ++i) {
    EXPECT_TRUE(publisher.send_bytes(message));
    usleep(10000);
  }
  message.reset();
  ASSERT_TRUE(subscriber.receive_bytes(message));
  std::string received_string;
  message.copy_to(received_string);
  EXPECT_STREQ(received_string.c_str(), send_string.c_str());
  publisher.close();
  subscriber.close();
}
