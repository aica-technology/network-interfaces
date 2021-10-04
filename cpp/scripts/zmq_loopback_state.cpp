#include <unistd.h>
#include <state_representation/robot/JointState.hpp>

#include "network_interfaces/zmq/network.h"

int main(int argc, char** argv) {
  std::string state_uri, command_uri;
  if (argc != 3) {
    std::cerr << "Provide two arguments: state_uri command_uri" << std::endl;
    return 1;
  } else {
    state_uri = argv[1];
    command_uri = argv[2];
  }
  ::zmq::context_t context(1);
  ::zmq::socket_t subscriber, publisher;
  network_interfaces::zmq::configure_subscriber(context, subscriber, state_uri, true);
  network_interfaces::zmq::configure_publisher(context, publisher, command_uri, true);

  network_interfaces::zmq::StateMessage state;
  network_interfaces::zmq::CommandMessage command;

  while (true) {
    if (network_interfaces::zmq::receive(state, subscriber)) {
      std::cout << state << std::endl;
      command.joint_state = state_representation::JointState::Zero(state.joint_state.get_name(), state.joint_state.get_names());
      command.joint_state.set_positions(state.joint_state.get_positions());
      command.control_type = std::vector<int>{3};
      network_interfaces::zmq::send(command, publisher);
    }
    usleep(100000);
  }

  return 0;
}
