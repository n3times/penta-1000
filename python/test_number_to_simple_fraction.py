import fileinput

from number2fraction import number_to_displayable_simple_fraction
from number2fraction import format_fraction

def string2number(s):
    a = int(s[0:15])
    b = int(s[15:18])
    return a * pow(10, b - 13)
    
def test():
    for line in fileinput.input():
        line = line.rstrip()
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
            result = number_to_displayable_simple_fraction(number)
            if (result == None):
                fraction = '.'
            else:
                fraction = format_fraction(result)
            print(_in, SEPARATOR, fraction, sep='')

test()
