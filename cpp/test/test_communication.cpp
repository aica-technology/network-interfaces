#include <gtest/gtest.h>
#include <thread>

#include "network_interfaces/zmq/network.h"

class TestNetworkInterface : public ::testing::Test {
public:
  TestNetworkInterface() {
    robot_state = state_representation::CartesianState::Random("ee", "robot");
    robot_joint_state = state_representation::JointState::Random("robot", 3);
    control_command = state_representation::JointState::Random("robot", 3);
    control_type = std::vector<int>{1, 2, 3};
  }

  state_representation::CartesianState robot_state;
  state_representation::JointState robot_joint_state;
  state_representation::JointState control_command;
  std::vector<int> control_type;

  std::thread spawn_robot() {
    return std::thread([this] { this->robot(); });
  }

  void robot() const {
    zmq::context_t context = zmq::context_t(1);
    zmq::socket_t state_publisher, command_subscriber;
    network_interfaces::zmq::configure_sockets(
        context, command_subscriber, "127.0.0.1:1702", state_publisher, "127.0.0.1:1701"
    );

    network_interfaces::zmq::CommandMessage command;
    network_interfaces::zmq::StateMessage state;
    state.ee_state = robot_state;
    state.joint_state = robot_joint_state;
    for (std::size_t i = 0; i < 100; ++i) {
      network_interfaces::zmq::send(state, state_publisher);
      network_interfaces::zmq::poll(command, command_subscriber);
      usleep(10000);
    }
    EXPECT_TRUE(command.joint_state.data().isApprox(control_command.data()));
    EXPECT_TRUE(command.joint_state.is_compatible(control_command));
    for (std::size_t i = 0; i < control_type.size(); ++i) {
      EXPECT_EQ(control_type.at(i), command.control_type.at(i));
    }
  }

  std::thread spawn_control() {
    return std::thread([this] { this->control(); });
  }

  void control() const {
    zmq::context_t context = zmq::context_t(1);
    zmq::socket_t command_publisher, state_subscriber;
    network_interfaces::zmq::configure_sockets(
        context, state_subscriber, "127.0.0.1:1701", command_publisher, "127.0.0.1:1702"
    );

    network_interfaces::zmq::StateMessage state;
    network_interfaces::zmq::CommandMessage command;
    command.joint_state = control_command;
    command.control_type = control_type;
    for (std::size_t i = 0; i < 100; ++i) {
      network_interfaces::zmq::send(command, command_publisher);
      network_interfaces::zmq::poll(state, state_subscriber);
      usleep(10000);
    }
    EXPECT_TRUE(state.ee_state.data().isApprox(robot_state.data()));
    EXPECT_TRUE(state.ee_state.is_compatible(robot_state));
    EXPECT_TRUE(state.joint_state.data().isApprox(robot_joint_state.data()));
    EXPECT_TRUE(state.joint_state.is_compatible(robot_joint_state));
  }
};

TEST_F(TestNetworkInterface, TestCommunication) {
  std::thread robot = spawn_robot();
  std::thread control = spawn_control();

  robot.join();
  control.join();
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}