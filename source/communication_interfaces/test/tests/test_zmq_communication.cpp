#include <gtest/gtest.h>
#include <thread>

#include "communication_interfaces/sockets/ZMQPublisher.hpp"
#include "communication_interfaces/sockets/ZMQSubscriber.hpp"

using namespace communication_interfaces;
using namespace std::chrono_literals;

class TestZMQSockets : public ::testing::Test {
public:
  TestZMQSockets() {
      params_.emplace_back(state_representation::make_shared_parameter<std::string>("ip_address", "127.0.0.1"));
      params_.emplace_back(state_representation::make_shared_parameter<std::string>("port", "5000"));
      context_ = std::make_shared<zmq::context_t>(1);
    }

    std::shared_ptr<zmq::context_t> context_;
    state_representation::ParameterInterfaceList params_;
};

TEST_F(TestZMQSockets, SendReceive) {
  const std::string send_string = "Hello world!";

  this->params_.emplace_back(state_representation::make_shared_parameter<bool>("bind_socket", true));
  sockets::ZMQPublisher publisher(this->params_, this->context_);

  this->params_.pop_back();
  this->params_.emplace_back(state_representation::make_shared_parameter<bool>("bind_socket", false));
  sockets::ZMQSubscriber subscriber(this->params_, this->context_);

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
