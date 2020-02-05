from number2fraction import number_to_displayable_mixed_fraction
from number2fraction import number_to_displayable_simple_fraction

MANT_DIGIT_COUNT = 14
FLO_DIGIT_COUNT = 10
SCI_DIGIT_COUNT = 7

def TEN(power):
    value = pow(10, power)
    return value

MANT_1 = TEN(MANT_DIGIT_COUNT - 1)
MANT_5 = 5 * MANT_1
MANT_10 = 10 * MANT_1

FLO_MANT_OVERFLOW = MANT_10 - TEN(MANT_DIGIT_COUNT - FLO_DIGIT_COUNT) // 2
SCI_MANT_OVERFLOW = MANT_10 - TEN(MANT_DIGIT_COUNT - SCI_DIGIT_COUNT) // 2

class Mode:
    FLO = 1
    SCI = 2
    ENG = 3
    MIX = 4
    SIM = 5

def fraction_to_string(f):
    if len(f) == 1:
        return str(f[0])
    elif len(f) == 2:
        return str(f[0]) + '/' + str(f[1])
    else:
        return str(f[0]) + ' ' + str(f[1]) + '/' + str(f[2])

# Given a number x, return display.
def number_to_display(mant, exp, mode):
    is_sci = mode == Mode.SCI or mode == Mode.ENG

    if mant == 0:
        if is_sci:
            return '0. 00'
        else:
            return '0.'

    number = mant * TEN(exp) / MANT_1
    if mode == Mode.MIX:
        value = number_to_displayable_mixed_fraction(number)
        if value == None or len(value) == 1:
            mode = Mode.FLO
        else:
            return fraction_to_string(value)
    elif mode == Mode.SIM:
        value = number_to_displayable_simple_fraction(number)
        if value == None or len(value) == 1:
            mode == Mode.FLO
        else:
            return fraction_to_string(value)

    abs_mant = abs(mant)

    if abs_mant >= SCI_MANT_OVERFLOW and exp == 99:
        output = (TEN(SCI_DIGIT_COUNT) - 1) / TEN(SCI_DIGIT_COUNT - 1)
        if mant < 0:
            output = '-' + str(output)
        return str(output) + ' 99'

    # Decide whether to use scientific notation.
    is_small = exp <= -FLO_DIGIT_COUNT - 2 \
        or (exp == -FLO_DIGIT_COUNT - 1 and abs_mant <= MANT_5 - 1)
    is_big = exp >= FLO_DIGIT_COUNT \
        or (exp == FLO_DIGIT_COUNT - 1 \
            and abs_mant >= FLO_MANT_OVERFLOW)
    if is_small or is_big:
        is_sci = True

    # Determine the significant digits.
    if is_sci:
        significant_digit_count = SCI_DIGIT_COUNT
    else:
        significant_digit_count = FLO_DIGIT_COUNT
        if exp < -1:
            significant_digit_count -= abs(exp) - 1
    significant_digits = \
        abs_mant // TEN(MANT_DIGIT_COUNT - significant_digit_count)
    next_digit = \
        abs_mant // TEN(MANT_DIGIT_COUNT - significant_digit_count - 1) % 10
    if next_digit >= 5:
        significant_digits += 1
    if significant_digits == TEN(significant_digit_count):
        significant_digits = 1
        significant_digit_count = 1
        exp += 1

    # Trim trailing zeros.
    if (significant_digit_count > 1):
        while significant_digits % 10 == 0:
            significant_digits //= 10
            significant_digit_count -= 1
    if significant_digit_count == 0:
        significant_digit_count = 1

    # Compute displayed mantissa.
    if is_sci:
        leading_digit_count = 1
        if mode == Mode.ENG:
            while exp % 3 != 0:
                leading_digit_count += 1
                exp -= 1
                if significant_digit_count < leading_digit_count:
                    significant_digits *= 10
                    significant_digit_count += 1
        digits_string = str(significant_digits)
        output = digits_string[0:leading_digit_count] + '.' \
            + digits_string[leading_digit_count:]
    elif exp >= 0:
        while significant_digit_count < exp + 1:
            significant_digits *= 10
            significant_digit_count += 1
        digits_string = str(significant_digits)
        output = digits_string[0:exp+1] + '.' + digits_string[exp+1:]
    else:
        output = str(significant_digits)
        for i in range(abs(exp) - 1):
            output = '0' + output
            significant_digit_count += 1
        output = '.' + output
        if significant_digit_count < FLO_DIGIT_COUNT:
            output = '0' + output

    # Add exponent.
    if is_sci:
        if exp < 0:
            output += '-'
        else:
            output += ' '
        if abs(exp) < 10:
            output += '0'
        output += str(abs(exp))

    if mant < 0:
        output = '-' + output

    return output

# print(number_to_display(-10000000000000, 2, Mode.FLO))
