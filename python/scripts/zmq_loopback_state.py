import argparse
import time

import zmq
from state_representation import CartesianState, JointState

from network_interfaces.zmq import network


def main(state_uri, command_uri):
    context = zmq.Context(1)
    subscriber = network.configure_subscriber(context, state_uri, True)
    publisher = network.configure_publisher(context, command_uri, True)

    command = network.CommandMessage()

    while True:
        state = network.receive_state(subscriber)
        if state:
            print(state)
            command.ee_state = CartesianState().Identity(state.ee_state.get_name(), state.ee_state.get_referene_frame())
            command.ee_state.set_pose(state.ee_state.get_pose())
            command.joint_state = JointState().Zero(state.joint_state.get_name(), state.joint_state.get_names())
            command.joint_state.set_positions(state.joint_state.get_positions())
            command.control_type = [3]
            network.send_command(command, publisher)
        time.sleep(0.1)

    exit(0)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("state_uri", type=str, help="URI of the state socket")
    parser.add_argument("command_uri", type=str, help="URI of the command socket")
    args = parser.parse_args()

    main(args.state_uri, args.command_uri)
