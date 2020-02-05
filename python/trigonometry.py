import math

from number import *
from number_double import *

class TrigMode:
    DEG = 1
    RAD = 2
    GRAD = 3

def convert_to_radians(n, mode):
    if abs(n.exp) >= 10:
        return OpResult(n, OpError.DATA_ERROR)
    x = n_to_x(n)
    if mode == TrigMode.GRAD:
        x = x / 400 * 2 * math.pi
    if mode == TrigMode.DEG:
        x = x / 360 * 2 * math.pi
    n = x_to_n(x)
    return OpResult(n, OpError.OK)

def convert_from_radians(x, mode):
    if mode == TrigMode.RAD:
        return x
    elif mode == TrigMode.GRAD:
        return x / 2 / math.pi * 400
    else:
        return x / 2 / math.pi * 360

    if abs(n.exp) >= 10:
        return OpResult(n, OpError.DATA_ERROR)
    x = n_to_x(n)
    if mode == TrigMode.GRAD:
        x = x / 400 * 2 * math.pi
    if mode == TrigMode.DEG:
        x = x / 360 * 2 * math.pi

    n = x_to_n(x)
    return OpResult(n, OpError.OK)
 
def cos(n, mode):
    result = convert_to_radians(n, mode)
    if result.error != OpError.OK:
        return OpResult(n, result.error)

    x = n_to_x(result.n)
    x = math.cos(x)

    n = x_to_n(x)
    return OpResult(n, OpError.OK)
 
def sin(n, mode):
    result = convert_to_radians(n, mode)
    if result.error != OpError.OK:
        return OpResult(n, result.error)

    x = n_to_x(result.n)
    x = math.sin(x)

    n = x_to_n(x)
    return OpResult(n, OpError.OK)

def tan(n, mode):
    result = convert_to_radians(n, mode)
    if result.error != OpError.OK:
        return OpResult(n, result.error)

    x = n_to_x(result.n)
    x = math.tan(x)

    n = x_to_n(x)
    return OpResult(n, OpError.OK)

def arcsin(n, mode):
    x = n_to_x(n)
    if abs(x) > 1:
        return OpResult(n, OpError.DATA_ERROR)

    x = math.asin(x)
    x = convert_from_radians(x, mode)

    n = x_to_n(x)
    return OpResult(n, OpError.OK)

def arccos(n, mode):
    x = n_to_x(n)
    if abs(x) > 1:
        return OpResult(n, OpError.DATA_ERROR)

    x = math.acos(x)
    x = convert_from_radians(x, mode)

    n = x_to_n(x)
    return OpResult(n, OpError.OK)

def arctan(n, mode):
    x = n_to_x(n)

    x = math.atan(x)
    x = convert_from_radians(x, mode)

    n = x_to_n(x)
    return OpResult(n, OpError.OK)

"""

Direct Tests.

"""

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

def test_base(cases, expect, mode, f):
    n = len(cases)
    assert(n == len(expect))
    for i in range(n):
        n = x_to_n(cases[i])
        result = f(n, mode) 
        actual = n_to_x(result.n)
        expected = expect[i]
        print(actual, expected)
        assert(approximately_equal(actual, expected))

S_2_2 = math.sqrt(2) / 2
INF = 1e99

RAD_CASES = [i * math.pi / 4 for i in range(9)]
DEG_CASES = [i * 45 for i in range(9)]
GRAD_CASES = [i * 50 for i in range(9)]

SIN_EXPECTED = [0, S_2_2, 1, S_2_2, 0, -S_2_2, -1, -S_2_2, 0]
COS_EXPECTED = [1, S_2_2, 0, -S_2_2, -1, -S_2_2, 0, S_2_2, 1]
TAN_EXPECTED = [0, 1, INF, -1, 0, 1, -INF, -1, 0]

def test_all_modes(f, expected):
    test_base(RAD_CASES, expected, TrigMode.RAD, f)
    test_base(DEG_CASES, expected, TrigMode.DEG, f)
    test_base(GRAD_CASES, expected, TrigMode.GRAD, f)

def test_direct():
    test_all_modes(cos, COS_EXPECTED)
    test_all_modes(sin, SIN_EXPECTED)
    test_all_modes(tan, TAN_EXPECTED)

"""

Arc Tests.

"""

PI = math.pi

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

def test_arc():
    test_base(ARC_COS_CASES, ARC_COS_RAD_EXPECTED, TrigMode.RAD, arccos)
    test_base(ARC_COS_CASES, ARC_COS_DEG_EXPECTED, TrigMode.DEG, arccos)
    test_base(ARC_COS_CASES, ARC_COS_GRAD_EXPECTED, TrigMode.GRAD, arccos)
    test_base(ARC_SIN_CASES, ARC_SIN_RAD_EXPECTED, TrigMode.RAD, arcsin)
    test_base(ARC_SIN_CASES, ARC_SIN_DEG_EXPECTED, TrigMode.DEG, arcsin)
    test_base(ARC_SIN_CASES, ARC_SIN_GRAD_EXPECTED, TrigMode.GRAD, arcsin)
    test_base(ARC_TAN_CASES, ARC_TAN_RAD_EXPECTED, TrigMode.RAD, arctan)
    test_base(ARC_TAN_CASES, ARC_TAN_DEG_EXPECTED, TrigMode.DEG, arctan)
    test_base(ARC_TAN_CASES, ARC_TAN_GRAD_EXPECTED, TrigMode.GRAD, arctan)

test_arc()
