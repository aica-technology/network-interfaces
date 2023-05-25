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
  std::string receive_string;

  sockets::ZMQPublisher publisher(this->config_);
  this->config_.bind = false;
  sockets::ZMQSubscriber subscriber(this->config_);

  publisher.open();
  subscriber.open();

  for (int i = 0; i < 5; ++i) {
    EXPECT_TRUE(publisher.send_bytes(send_string));
    usleep(10000);
  }
  ASSERT_TRUE(subscriber.receive_bytes(receive_string));
  EXPECT_STREQ(receive_string.c_str(), send_string.c_str());
  publisher.close();
  subscriber.close();
}
