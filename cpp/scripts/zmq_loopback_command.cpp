#include <unistd.h>
#include <state_representation/space/cartesian/CartesianState.hpp>
#include <state_representation/space/joint/JointState.hpp>

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
  network_interfaces::zmq::configure_subscriber(context, subscriber, command_uri, false);
  network_interfaces::zmq::configure_publisher(context, publisher, state_uri, false);

  network_interfaces::zmq::StateMessage state;
  state.joint_state = state_representation::JointState::Random("loopback", 7);
  state.ee_state = state_representation::CartesianState::Random("loopback_ee");
  network_interfaces::zmq::CommandMessage command;

  while (true) {
    network_interfaces::zmq::send(state, publisher);
    if (network_interfaces::zmq::receive(command, subscriber)) {
      std::cout << command << std::endl;
    }
    usleep(100000);
  }

  return 0;
}
