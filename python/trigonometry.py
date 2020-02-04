import math

from number import *
from number_double import *

class TrigMode:
    DEG = 1
    RAD = 2
    GRAD = 3

def convert_to_radians(n, mode):
    if abs(n.exp) >= 10:
        assert(1 == 0)
        return OpResult(n, False)
    x = n_to_x(n)
    if mode == TrigMode.GRAD:
        x = x / 400 * 2 * math.pi
    if mode == TrigMode.DEG:
        x = x / 360 * 2 * math.pi
    n = x_to_n(x)
    return OpResult(n, True)

def convert_from_radians(x, mode):
    if mode == TrigMode.RAD:
        return x
    elif mode == TrigMode.GRAD:
        return x / 2 / math.pi * 400
    else:
        return x / 2 / math.pi * 360

    if abs(n.exp) >= 10:
        assert(1 == 0)
        return OpResult(n, False)
    x = n_to_x(n)
    if mode == TrigMode.GRAD:
        x = x / 400 * 2 * math.pi
    if mode == TrigMode.DEG:
        x = x / 360 * 2 * math.pi

    n = x_to_n(x)
    return OpResult(n, True)
 
def cos(n, mode):
    result = convert_to_radians(n, mode)
    if not result.ok:
        return OpResult(n, False)

    x = n_to_x(result.n)
    x = math.cos(x)

    n = x_to_n(x)
    return OpResult(n, True)
 
def sin(n, mode):
    result = convert_to_radians(n, mode)
    if not result.ok:
        return OpResult(n, False)

    x = n_to_x(result.n)
    x = math.sin(x)

    n = x_to_n(x)
    return OpResult(n, True)

def tan(n, mode):
    result = convert_to_radians(n, mode)
    if not result.ok:
        return OpResult(n, False)

    x = n_to_x(result.n)
    x = math.tan(x)

    n = x_to_n(x)
    return OpResult(n, True)

def arcsin(n, mode):
    x = n_to_x(n)
    if abs(x) > 1:
        return OpResult(n, False)

    x = math.asin(x)
    x = convert_from_radians(x, mode)

    n = x_to_n(x)
    return OpResult(n, True)

def arccos(n, mode):
    x = n_to_x(n)
    if abs(x) > 1:
        return OpResult(n, False)

    x = math.acos(x)
    x = convert_from_radians(x, mode)

    n = x_to_n(x)
    return OpResult(n, True)

def arctan(n, mode):
    x = n_to_x(n)
    if abs(x) > 1:
        return OpResult(n, False)

    x = math.atan(x)
    x = convert_from_radians(x, mode)

    n = x_to_n(x)
    return OpResult(n, True)
