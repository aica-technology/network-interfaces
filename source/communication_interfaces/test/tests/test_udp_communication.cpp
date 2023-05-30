#include <gtest/gtest.h>

#include "communication_interfaces/exceptions/SocketConfigurationException.hpp"
#include "communication_interfaces/sockets/UDPClient.hpp"
#include "communication_interfaces/sockets/UDPServer.hpp"

using namespace communication_interfaces;

class TestUDPSockets : public ::testing::Test {
public:
  TestUDPSockets() {
    config_ = {"127.0.0.1", 5000, 100};
  }

  sockets::UDPSocketConfiguration config_;
};

TEST_F(TestUDPSockets, SendReceive) {
  const std::string send_string = "Hello world!";
  std::string receive_string;

  // Create server socket and bind it to a port
  sockets::UDPServer server(this->config_);
  ASSERT_NO_THROW(server.open());

  // Create client socket
  sockets::UDPClient client(this->config_);
  ASSERT_NO_THROW(client.open());

  // Send test message from client to server
  ASSERT_TRUE(client.send_bytes(send_string));

  // Receive message on server
  ASSERT_TRUE(server.receive_bytes(receive_string));

  // Convert received message to string and compare with sent message
  EXPECT_STREQ(receive_string.c_str(), send_string.c_str());
}

TEST_F(TestUDPSockets, Timeout) {
  // Create server socket and bind it to a port
  this->config_.timeout_duration_sec = 5.0;
  sockets::UDPServer server(this->config_);

  // Try to receive a message from client, but expect timeout
  std::string received_bytes;
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
  EXPECT_FALSE(server.send_bytes(std::string()));
}
