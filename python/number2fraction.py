"""

Public Functions

"""

# Converts x into a mixed fraction if possible.
def number_to_displayable_mixed_fraction(x):
    if x == 0:
        return (0,)
    is_negative = x < 0
    abs_x = abs(x)
    (a, b, c) = number2fraction(abs_x)
    fraction_is_accurate = dist(abs_x, a + b/c) <= 1e-11
    if (not fraction_is_accurate):
        return None
    fraction_fits_in_display = (a < 1000 and b < 1000 and c < 1000) \
        and (a < 100 or b < 100 or c < 100)
    if (not fraction_fits_in_display):
        return None
    if b == 0:
        if is_negative:
            a = -a
        return (a,)
    if a == 0:
        if is_negative:
            b = -b
        return (b, c)
    if is_negative:
        a = -a
    return (a, b, c) 

# Converts x into simple fraction if possible.
def number_to_displayable_simple_fraction(x):
    f = number_to_displayable_mixed_fraction(x)
    if f != None and len(f) == 3:
        if f[0] < 0:
            f = (f[0] * f[2] - f[1], f[2])
        else:
            f = (f[0] * f[2] + f[1], f[2])
    return f

# Utility method to convert a fraction of type a, a/b or a+b/c into string.
def format_fraction(f):
    count = len(f)
    if (count == 1):
        return str(f[0])
    elif (count == 2):
        return str(f[0]) + "/" + str(f[1])
    else:
        return str(f[0]) + " " + str(f[1]) + "/" + str(f[2])
 
"""

Private Functions

"""

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

def dist(x, y):
    return abs((x - y) / x)
