import threading
import time
import unittest

from network_interfaces.zmq import network
import state_representation as sr
import zmq
from numpy.testing import assert_array_almost_equal


class TestNetworkInterface(unittest.TestCase):
    robot_state = None
    robot_joint_state = None
    control_command = None
    control_type = None
    context = None
    received_state = None
    received_command = None

    @classmethod
    def setUpClass(cls):
        cls.robot_state = sr.CartesianState().Random("ee", "robot")
        cls.robot_joint_state = sr.JointState().Random("robot", 3)
        cls.control_command = sr.JointState().Random("robot", 3)
        cls.control_type = [1, 2, 3]
        cls.context = zmq.Context()

    def robot(self):
        command_subscriber, state_publisher = network.configure_sockets(self.context, "127.0.0.1:1702",
                                                                        "127.0.0.1:1701")

        state = network.StateMessage(self.robot_state, self.robot_joint_state)
        command = []
        for i in range(100):
            network.send_state(state, state_publisher)
            command = network.receive_command(command_subscriber)
            time.sleep(0.01)
        self.received_command = command

    def control(self):
        state_subscriber, command_publisher = network.configure_sockets(self.context, "127.0.0.1:1701",
                                                                        "127.0.0.1:1702")

        command = network.CommandMessage(self.control_type, self.control_command)
        state = []
        for i in range(100):
            network.send_command(command, command_publisher)
            state = network.receive_state(state_subscriber)
            time.sleep(0.01)
        self.received_state = state

    def test_communication(self):
        robot = threading.Thread(target=self.robot)
        control = threading.Thread(target=self.control)
        robot.start()
        control.start()
        robot.join()
        control.join()

        [self.assertEqual(self.received_command.control_type[i], self.control_type[i]) for i in
         range(len(self.control_type))]
        self.assertTrue(self.received_command.joint_state.is_compatible(self.control_command))
        assert_array_almost_equal(self.received_command.joint_state.data(), self.control_command.data())

        self.assertTrue(self.received_state.ee_state.is_compatible(self.robot_state))
        assert_array_almost_equal(self.received_state.ee_state.data(), self.robot_state.data())
        self.assertTrue(self.received_state.joint_state.is_compatible(self.robot_joint_state))
        assert_array_almost_equal(self.received_state.joint_state.data(), self.robot_joint_state.data())
