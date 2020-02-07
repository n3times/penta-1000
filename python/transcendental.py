import math

from number import *
from number_double import *

def log(n):
    x = x_from_n(n)
    if x <= 0:
        return OpResult(n, OpError.DATA_ERROR)

    x = math.log10(x)

    n = n_from_x(x)
    return OpResult(n, OpError.OK)

def ln(n):
    x = x_from_n(n)
    if x <= 0:
        return OpResult(n, OpError.DATA_ERROR)

    x = math.log(x)

    n = n_from_x(x)
    return OpResult(n, OpError.OK)

def pow10(n):
    x = x_from_n(n)
    if x >= 100:
        return OpResult(n, OpError.OVERFLOW)

    x = math.pow(10, x)

    n = n_from_x(x)
    return OpResult(n, OpError.OK)

def e(n):
    x = x_from_n(n)
    if x >= math.log(1e100):
        return OpResult(n, OpError.OVERFLOW)

    x = math.exp(x)

    n = n_from_x(x)
    return OpResult(n, OpError.OK)

"""

Tests.

"""

TEST_CASES = [
    -9.9999999999998e99,
    -1e10,
    -10,
    -1,
    -0.1,
    -1e-10,
    -1e-99,
    0,
    1e-99,
    1e-10,
    0.1,
    1,
    10,
    1e10,
    9.9999999999998e99,
]

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

def test_base(f, expect):
    assert(len(TEST_CASES) == len(expect))
    n = len(TEST_CASES)
    for i in range(n):
        n = x_to_n(TEST_CASES[i])
        result = f(n) 
        actual = n_to_x(result.n)
        expected = expect[i]
        print(actual, expected)
        assert(approximately_equal(actual, expected))

EXPECTED_LOG = [
    -9.9999999999998e99,
    -1e10,
    -10,
    -1,
    -0.1,
    -1e-10,
    -1e-99,
    0,
    -99,
    -10,
    -1,
    0,
    1,
    10,
    100,
]

EXPECTED_LN = [
    -9.9999999999998e99,
    -1e10,
    -10,
    -1,
    -0.1,
    -1e-10,
    -1e-99,
    0,
    -227.95592420640,
    -23.02585092994,
    -2.302585092994,
    0,
    2.302585092994,
    23.02585092994,
    230.25850929939
]

EXPECTED_POW10 = [
    0,
    0,
    1e-10,
    0.1,
    0.79432823472428,
    0.99999999976974,
    1,
    1,
    1,
    1.0000000002303,
    1.2589254117942001,
    10,
    1e10,
    1e10,
    9.9999999999998e99,
]

EXPECTED_E = [
    0.0,
    0.0,
    4.5399929762485e-05,
    0.36787944117144,
    0.90483741803596,
    0.9999999999,
    1.0,
    1.0,
    1.0,
    1.0000000001,
    1.1051709180756,
    2.7182818284590002,
    22026.465794807,
    10000000000.0,
    9.9999999999999e99
]

def test_log():
    test_base(log, EXPECTED_LOG)

def test_ln():
    test_base(ln, EXPECTED_LN)

def test_pow10():
    test_base(pow10, EXPECTED_POW10)

def test_e():
    test_base(e, EXPECTED_E)

def test():
    test_log()
    test_ln()
    test_pow10()
    test_e()

test()

