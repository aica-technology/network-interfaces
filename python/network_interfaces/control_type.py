from enum import Enum


class ControlType(Enum):
    """
    An enumeration with possible control types.
    """
    UNDEFINED = 0
    POSITION = 1
    VELOCITY = 2
    ACCELERATION = 3
    EFFORT = 4
