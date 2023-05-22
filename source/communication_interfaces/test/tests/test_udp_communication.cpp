#include <gtest/gtest.h>

#include "communication_interfaces/exceptions/SocketConfigurationException.hpp"
#include "communication_interfaces/sockets/UDPClient.hpp"
#include "communication_interfaces/sockets/UDPServer.hpp"

using namespace communication_interfaces;

class TestUDPSockets : public ::testing::Test {
public:
  TestUDPSockets() {
    config_ = {.ip_address = "127.0.0.1", .port = 5000, .buffer_size = 100};
  }

  sockets::UDPSocketConfiguration config_{};
};

TEST_F(TestUDPSockets, SendReceive) {
  const std::string send_string = "Hello world!";

  // Create server socket and bind it to a port
  sockets::UDPServer server(this->config_);
  ASSERT_NO_THROW(server.open());

  // Create client socket
  sockets::UDPClient client(this->config_);
  ASSERT_NO_THROW(client.open());

  // Send test message from client to server
  ByteArray message;
  message.copy_from(send_string);
  ASSERT_TRUE(client.send_bytes(message));

  // Receive message on server
  message.reset();
  ASSERT_TRUE(server.receive_bytes(message));

  // Convert received message to string and compare with sent message
  std::string received_string;
  message.copy_to(received_string);
  EXPECT_STREQ(received_string.c_str(), send_string.c_str());
}

TEST_F(TestUDPSockets, Timeout) {
  // Create server socket and bind it to a port
  this->config_.timeout_duration_sec = 5.0;
  sockets::UDPServer server(this->config_);

  // Try to receive a message from client, but expect timeout
  ByteArray received_bytes;
  EXPECT_FALSE(server.receive_bytes(received_bytes));
}

TEST_F(TestUDPSockets, PortReuse) {
  // Create server socket and bind it to a port
  sockets::UDPServer server1(this->config_);
  server1.open();

  // Try to create a second server socket and bind it to the same port (expect failure)
  sockets::UDPServer server2(this->config_);
  EXPECT_THROW(server2.open(), exceptions::SocketConfigurationException);
}

TEST_F(TestUDPSockets, OpenClose) {
  // Create and open server socket
  sockets::UDPServer server(this->config_);
  server.open();

  // Close server socket
  server.close();

  // Create and open client socket
  sockets::UDPClient client(this->config_);
  client.open();

  // Try to send a message from the closed server socket (expect failure)
  EXPECT_FALSE(server.send_bytes(ByteArray()));
}
