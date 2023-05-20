#include <gtest/gtest.h>
#include <thread>

#include "communication_interfaces/sockets/ZMQPublisher.hpp"
#include "communication_interfaces/sockets/ZMQSubscriber.hpp"

using namespace communication_interfaces;
using namespace std::chrono_literals;

class TestZMQSockets : public ::testing::Test {
public:
  TestZMQSockets() {
    ip_address_ = "127.0.0.1";
    port_ = "5000";
    context_ = std::make_shared<zmq::context_t>(1);
  }

  std::shared_ptr<zmq::context_t> context_;
  std::string ip_address_;
  std::string port_;
};

TEST_F(TestZMQSockets, SendReceive) {
  const std::string send_string = "Hello world!";

  sockets::ZMQPublisher publisher({this->ip_address_, this->port_, true}, this->context_);
  sockets::ZMQSubscriber subscriber({this->ip_address_, this->port_, false}, this->context_);

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
