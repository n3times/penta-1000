import fileinput

from number2display import number_to_display
from number2display import Mode

def test(mode):
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
            a = int(_in[0:15])
            b = int(_in[15:18])
            result = number_to_display(a, b, mode)
            print(_in, SEPARATOR, result, sep='')
