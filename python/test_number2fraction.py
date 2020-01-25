import fileinput

from number2fraction import number2fraction
from number2fraction import format_fraction

def string2number(s):
    is_exp_negative = '-' in s
    separator = ' '
    if is_exp_negative:
        separator = '-'
    a, b = s.split(separator)
    a = int(a)
    b = int(b)
    if is_exp_negative:
        b = -b
    return a * pow(10, b - 13)
    
def test():
    for line in fileinput.input():
        line = line.strip()
        is_comment = line[0] == '#'
        if is_comment:
            print(line)
        else:
            SEPARATOR = ' => '
            if SEPARATOR in line:
                _in, dummy = line.split(SEPARATOR) 
            else:
                _in = line
            number = string2number(_in)
            result = number2fraction(number)
            fraction = format_fraction(number, result)
            print(_in, SEPARATOR, fraction, sep='')

test()
