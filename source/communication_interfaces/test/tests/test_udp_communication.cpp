#include <gtest/gtest.h>

#include "communication_interfaces/exceptions/SocketConfigurationException.hpp"
#include "communication_interfaces/sockets/UDPClient.hpp"
#include "communication_interfaces/sockets/UDPServer.hpp"

using namespace communication_interfaces;

class TestUDPSockets : public ::testing::Test {
public:
  TestUDPSockets() {
    ip_address_ = "127.0.0.1";
    port_ = 5000;
    buffer_size_ = 100;
  }

  std::string ip_address_;
  int port_;
  int buffer_size_;
};

TEST_F(TestUDPSockets, SendReceive) {
  const std::string send_string = "Hello world!";

  // Create server socket and bind it to a port
  sockets::UDPServer server({this->ip_address_, this->port_, this->buffer_size_});
  ASSERT_NO_THROW(server.open());

  // Create client socket
  sockets::UDPClient client({this->ip_address_, this->port_, this->buffer_size_});
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
  sockets::UDPServer server({this->ip_address_, this->port_, this->buffer_size_, false, 5.0});

  // Try to receive a message from client, but expect timeout
  ByteArray received_bytes;
  EXPECT_FALSE(server.receive_bytes(received_bytes));
}

TEST_F(TestUDPSockets, PortReuse) {
  // Create server socket and bind it to a port
  sockets::UDPServer server1({this->ip_address_, this->port_, this->buffer_size_});
  server1.open();

  // Try to create a second server socket and bind it to the same port (expect failure)
  sockets::UDPServer server2({this->ip_address_, this->port_, this->buffer_size_});
  EXPECT_THROW(server2.open(), exceptions::SocketConfigurationException);
}

TEST_F(TestUDPSockets, OpenClose) {
  // Create and open server socket
  sockets::UDPServer server({this->ip_address_, this->port_, this->buffer_size_});
  server.open();

  // Close server socket
  server.close();

  // Create and open client socket
  sockets::UDPClient client({this->ip_address_, this->port_, this->buffer_size_});
  client.open();

  // Try to send a message from the closed server socket (expect failure)
  EXPECT_FALSE(server.send_bytes(ByteArray()));
}
