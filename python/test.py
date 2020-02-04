from number import Number
from number_double import n_to_x
from arithmetic import *
from trigonometry import *

a = Number(20000000000000, 0)
b = Number(30000000000000, 0)

result = add(a, b)
print(n_to_x(result.n))
result = substract(a, b)
print(n_to_x(result.n))
result = multiply(a, b)
print(n_to_x(result.n))
result = divide(a, b)
print(n_to_x(result.n))
result = power(a, b)
print(n_to_x(result.n))

result = cos(x_to_n(30), TrigMode.DEG)
print(n_to_x(result.n))
result = sin(x_to_n(30), TrigMode.DEG)
print(n_to_x(result.n))
result = tan(x_to_n(30), TrigMode.DEG)
print(n_to_x(result.n))

result = arcsin(x_to_n(0.5), TrigMode.DEG)
print(n_to_x(result.n))
result = arccos(x_to_n(0.5), TrigMode.DEG)
print(n_to_x(result.n))
result = arctan(x_to_n(1), TrigMode.DEG)
print(n_to_x(result.n))
