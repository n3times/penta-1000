import math

from number import Number

DIGIT_COUNT = 14
TEN = pow(10, DIGIT_COUNT)

def n_to_x(n):
    return n.mant * pow(10, n.exp - DIGIT_COUNT + 1)

def x_from_n(n):
    return n_to_x(n)

def x_to_n(x):
    if x == 0:
        return Number(0, 0)

    is_negative = x < 0
    x = abs(x)

    exp = math.floor(math.log10(x))
    mant_10 = int(x // pow(10, exp - DIGIT_COUNT))
    next_digit = mant_10 % 10
    mant = mant_10 // 10
    if next_digit >= 5:
        mant += 1
    if mant == TEN:
        mant //= 10
        exp += 1
    if is_negative:
        mant = -mant

    return Number(mant, exp)

def n_from_x(x):
    return x_to_n(x)
