import math

DIGIT_COUNT = 14
ONE = pow(10, DIGIT_COUNT - 1)
TEN = pow(10, DIGIT_COUNT)

class Number:
    mant = 0
    exp = 0

    def __init__(self, mant, exp):
        assert(abs(mant) == 0 or abs(mant) >= ONE)
        assert(abs(mant) < TEN)
        assert(abs(exp) <= 99)
        self.mant = mant
        self.exp = exp

class OpResult:
    n = 0
    ok = True

    def __init__(self, n, ok):
        self.n = n
        self.ok = ok
