import math

from number import *
from number_double import *

class TrigMode:
    DEG = 1
    RAD = 2
    GRAD = 3

def cos(n, mode):
    return trig(math.cos, n, mode)
 
def sin(n, mode):
    return trig(math.sin, n, mode)

def tan(n, mode):
    return trig(math.tan, n, mode)

def arcsin(n, mode):
    return arctrig(math.asin, n, mode)

def arccos(n, mode):
    return arctrig(math.acos, n, mode)

def arctan(n, mode):
    return arctrig(math.atan, n, mode)

"""

Utility functions.

"""

def trig(f, n, mode):
    x = n_to_x(n)
    x = reduce_angle(x, mode)
    x = convert_to_radians(x, mode)

    x = f(x)

    n = x_to_n(x)
    return OpResult(n, OpError.OK)

def arctrig(f, n, mode):
    x = n_to_x(n)
    if f == math.asin or f == math.acos:
        if abs(x) > 1:
            return OpResult(n, OpError.DATA_ERROR)

    x = f(x)

    x = convert_from_radians(x, mode)
    n = x_to_n(x)
    return OpResult(n, OpError.OK)

def reduce_angle(x, mode):
    if mode == TrigMode.RAD:
        x = x % (2 * math.pi)
    elif mode == TrigMode.DEG:
        x = x % 360
    elif mode == TrigMode.GRAD:
        x = x % 400
    return x

def convert_to_radians(x, mode):
    if mode == TrigMode.GRAD:
        x = 2 * math.pi * (x / 400)
    elif mode == TrigMode.DEG:
        x = 2 * math.pi * (x / 360)
    return x

def convert_from_radians(x, mode):
    if mode == TrigMode.GRAD:
        x = x / 2 / math.pi * 400
    elif mode == TrigMode.DEG:
        x = x / 2 / math.pi * 360
    return x
"""

Tests.

"""
S_2_2 = math.sqrt(2) / 2
INF = 1e99
PI = math.pi

RAD_CASES = [i * math.pi / 4 for i in range(9)]
DEG_CASES = [i * 45 for i in range(9)]
GRAD_CASES = [i * 50 for i in range(9)]

SIN_EXPECTED = [0, S_2_2, 1, S_2_2, 0, -S_2_2, -1, -S_2_2, 0]
COS_EXPECTED = [1, S_2_2, 0, -S_2_2, -1, -S_2_2, 0, S_2_2, 1]
TAN_EXPECTED = [0, 1, INF, -1, 0, 1, -INF, -1, 0]

ARC_SIN_CASES = [-1, -S_2_2, 0, S_2_2, 1]
ARC_COS_CASES = [-1, -S_2_2, 0, S_2_2, 1]
ARC_TAN_CASES = [-INF, -1 , 0, 1, INF]

ARC_SIN_RAD_EXPECTED = [-PI/2, -PI/4, 0, PI/4, PI/2]
ARC_SIN_DEG_EXPECTED = [-90, -45, 0, 45, 90]
ARC_SIN_GRAD_EXPECTED = [-100, -50, 0, 50, 100]
ARC_COS_RAD_EXPECTED = [PI, 3*PI/4, PI/2, PI/4, 0]
ARC_COS_DEG_EXPECTED = [180, 135, 90, 45, 0]
ARC_COS_GRAD_EXPECTED = [200, 150, 100, 50, 0]
ARC_TAN_RAD_EXPECTED = [-PI/2, -PI/4, 0, PI/4, PI/2]
ARC_TAN_DEG_EXPECTED = [-90, -45, 0, 45, 90]
ARC_TAN_GRAD_EXPECTED = [-100, -50, 0, 50, 100]

def approximately_equal(actual, expected):
    if abs(actual) > 1e10 and abs(expected) > 1e10:
        return True
    if abs(actual) < 1e-11 and abs(expected) < 1e-11:
        return True
    if abs(actual) >= 1e-11 and abs(expected) < 1e-11:
        return False
    if abs(actual) < 1e-11 and abs(expected) >= 1e-11:
        return False
    if abs(1 - actual/expected) < 1e-11:
        return True
    return False

def test_base(f, mode, cases, expect):
    assert(len(cases) == len(expect))
    n = len(cases)
    for i in range(n):
        n = x_to_n(cases[i])
        result = f(n, mode) 
        actual = n_to_x(result.n)
        expected = expect[i]
        print(actual, expected)
        assert(approximately_equal(actual, expected))

def test_cases():
    # Direct.
    test_base(cos, TrigMode.RAD, RAD_CASES, COS_EXPECTED)
    test_base(cos, TrigMode.DEG, DEG_CASES, COS_EXPECTED)
    test_base(cos, TrigMode.GRAD, GRAD_CASES, COS_EXPECTED)
    test_base(sin, TrigMode.RAD, RAD_CASES, SIN_EXPECTED)
    test_base(sin, TrigMode.DEG, DEG_CASES, SIN_EXPECTED)
    test_base(sin, TrigMode.GRAD, GRAD_CASES, SIN_EXPECTED)
    test_base(tan, TrigMode.RAD, RAD_CASES, TAN_EXPECTED)
    test_base(tan, TrigMode.DEG, DEG_CASES, TAN_EXPECTED)
    test_base(tan, TrigMode.GRAD, GRAD_CASES, TAN_EXPECTED)

    # Arc.
    test_base(arccos, TrigMode.RAD, ARC_COS_CASES, ARC_COS_RAD_EXPECTED)
    test_base(arccos, TrigMode.DEG, ARC_COS_CASES, ARC_COS_DEG_EXPECTED)
    test_base(arccos, TrigMode.GRAD, ARC_COS_CASES, ARC_COS_GRAD_EXPECTED)
    test_base(arcsin, TrigMode.RAD, ARC_SIN_CASES, ARC_SIN_RAD_EXPECTED)
    test_base(arcsin, TrigMode.DEG, ARC_SIN_CASES, ARC_SIN_DEG_EXPECTED)
    test_base(arcsin, TrigMode.GRAD, ARC_SIN_CASES, ARC_SIN_GRAD_EXPECTED)
    test_base(arctan, TrigMode.RAD, ARC_TAN_CASES, ARC_TAN_RAD_EXPECTED)
    test_base(arctan, TrigMode.DEG, ARC_TAN_CASES, ARC_TAN_DEG_EXPECTED)
    test_base(arctan, TrigMode.GRAD, ARC_TAN_CASES, ARC_TAN_GRAD_EXPECTED)

test_cases()
