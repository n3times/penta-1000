# Converts x >= 0 into a fraction f.
#
# f = a + b / c should verify the following conditions:
# - 0 <= b < c <= 999
# - f and x should be as close as possible
#
# Returns f = (a, b, c).
def number2fraction(x):
    assert(x >= 0)
    a = int(x)
    b, c = farey(x - a, 999)
    if b == 1 and c == 1:
        a += 1
        b = 0
    f = (a, b, c)
    return f

# Converts 0 <= x < 1 into a fraction f = a / b such that:
# - 0 <= a <= b <= max
# - f and x should be as close as possible
#
# Returns f = (a, b).
def farey(x, max):
    assert(x >= 0 and x < 1)
    a, b = 0, 1
    c, d = 1, 1
    while (b + d <= max):
        e = a + c
        f = b + d
        y = float(e) / f
        if (y < x):
            a, b = e, f
        else:
            c, d = e, f
    if abs(x - a/b) <= abs(x - c/d):
        return (a, b)
    else:
        return (c, d)

def dist(x, y):
    return abs((x - y) / x)

def print_result(number, result):
    a, b, c = result
    d = dist(number, a+b/c)
    if (d > 1e-11):
        print(number, "=", number)
    elif (b == 0):
        print(number, "=", a)
    elif (a == 0):
        print(number, "=", b, "/", c)
    else:
        print(number, "=", a, "+", b, "/", c)

def test():
    fractions = [
        1.0/2,
        1.0/3,
        1.0/13,
        1.0/113,
        113.0/355
    ]
    for fraction in fractions:
        for number in [1000, 100, 10, 1]:
            x = number + fraction
            result = number2fraction(x)
            print_result(x, result)
    for number in [1, 10, 100, 1000]:
        for fraction in fractions:
            x = fraction / number
            result = number2fraction(x)
            print_result(x, result)
    numbers = [
        1.0/2 + 1.0/3 + 1.0/5 + 1.0/7,
        9.9999999999999,
        0.1231421431,
        999 + 11.0/111
    ]
    for number in numbers:
        result = number2fraction(number)
        print_result(number, result)
    numbers = [
        0.111111111110, 
        0.111111111111, 
        0.111111111112, 
        0.111111111113, 
        0.333333333330,
        0.333333333331,
        0.333333333332,
        0.333333333333,
        0.333333333334,
        0.333333333335,
        0.333333333336,
        0.333333333337,
        3
    ]
    for number in numbers:
        result = number2fraction(number)
        print_result(number, result)

test()
