from enum import Enum


class ControlType(Enum):
    """
    An enumeration with possible control types.
    """
    UNDEFINED = 0
    JOINT_POSITION = 1
    JOINT_VELOCITY = 2
    JOINT_ACCELERATION = 3
    JOINT_EFFORT = 4
    CARTESIAN_POSE = 5
    CARTESIAN_TWIST = 6
    CARTESIAN_ACCELERATION = 7
    CARTESIAN_WRENCH = 8
    END = 9
