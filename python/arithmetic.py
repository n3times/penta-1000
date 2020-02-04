import math

from number import *
from number_double import *

MAX = 9.9999999999999e99

def sign(x):
    if x == 0:
        return 0
    if x < 0:
        return -1
    return 1

def relatively_equal(x1, x2):
    if x2 == 0:
        return x1 == x2
    error = (x1 - x2)/x2
    threshold = 1e-11
    if abs(error) >= threshold:
        return False
    return True
 
def add(n1, n2):
    x1 = n_to_x(n1)
    x2 = n_to_x(n2)

    if sign(x1) * sign(x2) < 0 and relatively_equal(x1, -x2):
        x = 0
    else:
        x = x1 + x2

    n = x_to_n(x)
    return OpResult(n, True)

def substract(n1, n2):
    x1 = n_to_x(n1)
    x2 = n_to_x(n2)

    if sign(x1) * sign(x2) > 0 and relatively_equal(x1, x2):
        x = 0
    else:
        x = x1 - x2

    n = x_to_n(x)
    return OpResult(n, True)

def multiply(n1, n2):
    x1 = n_to_x(n1)
    x2 = n_to_x(n2)

    x = x1 * x2

    n = x_to_n(x)
    return OpResult(n, True)

def divide(n1, n2):
    x1 = n_to_x(n1)
    x2 = n_to_x(n2)

    if x2 == 0:
        ok = False
        if x1 == 0:
            x = 1
        elif x1 > 0:
            x = MAX
        else:
            x = -MAX
    else:
        ok = True
        x = x1 / x2

    n = x_to_n(x)
    return OpResult(n, ok)

def power(n1, n2):
    x1 = n_to_x(n1)
    x2 = n_to_x(n2)

    is_negative = x1 < 0 and x2 % 2 == 1
    ok = True
    if x1 < 0 and x2 % 1 != 0:
        ok = False
    x1 = abs(x1)
    exp = math.log10(x1) * x2
    if exp >= 100:
        x = MAX
    elif exp <= -100:
        x = 0
    else:
        x = pow(10, exp)
    if is_negative:
        x = -x

    n = x_to_n(x)
    return OpResult(n, True)
