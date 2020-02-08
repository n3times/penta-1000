from number import *
from arithmetic import *

class LeftParenthesis:
    LEFT_PARENTHESIS = 1

class OperatorType:
    PLUS = 1
    MINUS = 2 
    TIMES = 3
    DIVIDE = 4
    POW = 5
    INV_POW = 6

class Operator:
    def __init__(self, operatorType, priority):
        self.operatorType = operatorType
        self.priority = priority

Operator.PLUS = Operator(OperatorType.PLUS, 1)
Operator.MINUS = Operator(OperatorType.MINUS, 1)
Operator.TIMES = Operator(OperatorType.TIMES, 2)
Operator.DIVIDE = Operator(OperatorType.DIVIDE, 2)
Operator.POW = Operator(OperatorType.POW, 3)
Operator.INV_POW = Operator(OperatorType.INV_POW, 3)

class Stack:
    def __init__(self):
        self.elements = [ Number(0, 0) ]
    
    def push(self, element):
        assert not self.empty()
        if element == LeftParenthesis.LEFT_PARENTHESIS:
            return self.push_left_parenthesis(element)
        elif isinstance(element, Number):
            return self.push_number(element)
        elif isinstance(element, Operator):
            return self.push_operator(element)
        assert False

    def top(self):
        return self.elements[-1]

    def size(self):
        return len(self.elements)

    def empty(self):
        return self.size() == 0

    # Reduces stack down to the first left parenthesis encountered (included).
    # If no left parenthesis, reduces stack to the end, leaving result as
    # the only element of the stack.
    def reduce_right(self):
        while True:
            assert not self.empty()
            topElement = self.elements.pop()
            if not isinstance(topElement, Number):
                return False
            if self.empty():
                self.elements.append(topElement)
                return True
            assert not self.empty()
            nextElement = self.elements.pop()
            if nextElement == LeftParenthesis.LEFT_PARENTHESIS:
                self.elements.append(topElement)
                return True
            assert isinstance(nextElement, Operator)
            assert not self.empty()
            nextNextElement = self.elements.pop()
            assert isinstance(nextNextElement, Number)
            result = compute(nextNextElement, nextElement, topElement)
            self.elements.append(result)
       
    def reduce_all(self):
        while True:
            if self.size() == 1:
                return isinstance(self.elements[0], Number)
            ok = self.reduce_right()
            if not ok:
                return False 
            

    def push_left_parenthesis(self, leftParenthesis):
        if isinstance(self.top(), Number):
            if self.size() == 1:
                self.elements[0] = leftParenthesis
                return True
            else:
                return False

        self.elements.append(leftParenthesis)
        return True

    def push_number(self, number):
        if isinstance(self.top(), Number):
            # Remove top number to replace it by new one.
            self.elements.pop()
        self.elements.append(number)

        return True

    def push_operator(self, operator):
        if self.top() == LeftParenthesis.LEFT_PARENTHESIS:
            return False

        if isinstance(self.top(), Operator):
            # Remove top operator to replace it by new one.
            self.elements.pop()

        while True:
            print(self.size())
            assert self.size() > 0
            if self.size() == 1:
                # Done reducing.
                assert isinstance(self.top(), Number)
                break
            previousOperator = self.elements[-2]
            if previousOperator == LeftParenthesis.LEFT_PARENTHESIS:
                break 
            assert isinstance(previousOperator, Operator)
            assert self.size() >= 3
            if previousOperator.priority >= operator.priority:
                rightOperand = self.elements.pop()
                previousOperator = self.elements.pop()
                leftOperand = self.elements.pop()
                result = compute(leftOperand, previousOperator, rightOperand)
                self.elements.append(result)
            else:
                break

        self.elements.append(operator)
        return True

def compute(a, op, b):
    if op == Operator.PLUS:
        return add(a, b).n
    if op == Operator.MINUS:
        return substract(a, b).n
    if op == Operator.TIMES:
        return multiply(a, b).n
    if op == Operator.DIVIDE:
        return divide(a, b).n
    if op == Operator.POW:
        return power(a, b).n
    if op == Operator.INV_POW:
        return inv_power(a, b).n

def test():
    stack = Stack()
    stack.push(Number(20000000000000, 0))
    stack.push(Operator.TIMES)
    stack.push(Number(30000000000000, 0))
    stack.push(Operator.PLUS)
    stack.push(Number(50000000000000, 0))
    stack.push(Operator.POW)
    stack.push(Number(20000000000000, 0))
    stack.reduce_all()
    n = stack.top()
    print(n.mant)
