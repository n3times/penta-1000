# Converts a positive number into a fraction.
#
# The fraction f = a + b / c should verify the following conditions:
# - 0 <= b < c <= 999
# - f and x are as close as possible
#
# Returns f = (a, b, c).
def number2fraction(x):
    assert(x >= 0)
    a = int(x)
    b, c = convert(x - a, 999)
    if b == 1 and c == 1:
        a += 1
        b = 0
    f = (a, b, c)
    return f

# Converts 0 <= x < 1 into a fraction f = a / b such that:
# - 0 <= a <= b <= max
# - f and x are as close as possible
#
# Returns f = (a, b).
def convert(x, max):
    assert(x >= 0 and x < 1)
    a, b = 0, 1
    c, d = 1, 1
    while (b + d <= max):
        g = a + c
        h = b + d
        y = g / h
        if (y < x):
            a, b = g, h
        else:
            c, d = g, h
    if abs(x - a / b) <= abs(x - c / d):
        return (a, b)
    else:
        return (c, d)

# Returns a formatted version of fraction = (a, b, c) or '.' if the fraction is
# not accurate enough or does not fit in display.
def format_fraction(number, fraction):
    a, b, c = fraction
    fraction_is_accurate = dist(number, a + b/c) <= 1e-11
    if (not fraction_is_accurate):
        return '.'
    fraction_fits_in_display = (a < 1000 and b < 1000 and c < 1000) \
        and (a < 100 or b < 100 or c < 100)
    if (not fraction_fits_in_display):
        return '.'
    elif (b == 0):
        return a
    elif (a == 0):
        return str(b) + "/" + str(c)
    else:
        return str(a) + " " + str(b) + "/" + str(c)

def dist(x, y):
    return abs((x - y) / x)
