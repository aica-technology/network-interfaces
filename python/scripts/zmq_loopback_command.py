import argparse
import time

import zmq
from state_representation import CartesianState, JointState

from network_interfaces.zmq import network


def main(state_uri, command_uri):
    context = zmq.Context(1)
    subscriber = network.configure_subscriber(context, command_uri, False)
    publisher = network.configure_publisher(context, state_uri, False)

    state = network.StateMessage()
    state.joint_state = JointState().Random("loopback", 7)
    state.ee_state = CartesianState().Random("loopback_ee")

    while True:
        network.send_state(state, publisher)
        command = network.receive_command(subscriber)
        if command:
            print(command)
        time.sleep(0.1)

    exit(0)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("state_uri", type=str, help="URI of the state socket")
    parser.add_argument("command_uri", type=str, help="URI of the command socket")
    args = parser.parse_args()

    main(args.state_uri, args.command_uri)
