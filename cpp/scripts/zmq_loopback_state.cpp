#include <unistd.h>
#include <state_representation/robot/JointState.hpp>
#include <state_representation/space/cartesian/CartesianPose.hpp>
#include <dynamical_systems/Linear.hpp>
#include <controllers/impedance/CartesianTwistController.hpp>

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

  state_representation::CartesianPose target("panda_ee", "panda_base");
  target.set_position(.3, .4, .5);
  target.set_orientation(Eigen::Quaterniond(0, 1, 0, 0));
  std::vector<double> gains = {50.0, 50.0, 50.0, 10.0, 10.0, 10.0};
  dynamical_systems::Linear<state_representation::CartesianState> linear_ds(target, gains);

  controllers::impedance::CartesianTwistController ctrl(2, 2, .5, .5);

  while (true) {
    if (network_interfaces::zmq::receive(state, subscriber)) {
      state_representation::CartesianTwist twist = linear_ds.evaluate(state.ee_state);
      twist.clamp(1, 0.5);
      state.ee_state.set_wrench(Eigen::VectorXd::Zero(6));
      state_representation::JointTorques torques = ctrl.compute_command(twist, state.ee_state, state.jacobian);
      command.joint_state = state.joint_state;
      command.joint_state.set_torques(torques.data());
      command.control_type = std::vector<int>{4};
//      command.joint_state.set_velocities(state.jacobian.data().colPivHouseholderQr().solve(twist.data()));
//      command.control_type = std::vector<int>{2};
//      command.joint_state.set_positions(
//          state.joint_state.get_positions() + 0.01 * state.jacobian.data().colPivHouseholderQr().solve(twist.data()));
//      command.control_type = std::vector<int>{1};
      network_interfaces::zmq::send(command, publisher);
    }
    usleep(10000);
  }

  return 0;
}
