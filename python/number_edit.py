class NumberEdit:
    MAN = 1
    EXP = 2
    SIM = 3
    MIX = 4

    def __init__(self):
        self.state = NumberEdit.MAN
        self.is_neg = False
        self.is_exp_neg = False
        self.has_dot = False
        self.man = ''
        self.exp = ''
        self.sim = ''
        self.mix = ''

    def chs(self):
        if self.state == NumberEdit.EXP:
            self.is_exp_neg = not self.is_exp_neg
        else:
            self.is_neg = not self.is_neg

    def dot(self):
        if self.has_dot:
            pass
        elif self.state == NumberEdit.MAN:
            self.has_dot = True
            self.man += '.'

    def ee(self):
        if self.state == NumberEdit.MAN:
            digit_count = self.mant_digit_count()
            if 1 <= digit_count and digit_count <= 7:
                self.state = NumberEdit.EXP

    def fraction(self):
        if self.state == NumberEdit.MAN:
            if not has_dot and len(self.man) <= 8:
                self.state = NumberEdit.SIM
        elif self.state == NumberEdit.SIM:
            if len(self.man) + len(self.sim)  + 1 <= 8:
                self.state = NumberEdit.MIX

    def digit(self, digit):
        if self.state == NumberEdit.MAN:
            if self.mant_digit_count() < 10:
                self.man += digit
        elif self.state == NumberEdit.EXP:
            if len(self.exp) < 2:
                self.exp += digit
        elif self.state == NumberEdit.SIM:
            if len(self.man) + len(self.sim)  + 1 < 10 and len(self.sim) < 3:
                self.sim += digit
        elif self.state == NumberEdit.MIX:
            if len(self.man) + len(self.sim)  + len(self.mix) + 2 < 10 and len(self.mix) < 3:
                self.mix += digit

    def back(self):
        if self.state == NumberEdit.EXP:
            if len(self.exp) == 0:
                if self.is_exp_neg:
                    self.is_exp_neg = False
                else:
                    self.state = NumberEdit.MAN
            else:
                self.exp = self.exp[:-1]
        elif self.state == NumberEdit.MAN:
            if len(self.man) == 0:
                self.is_neg = False
            else:
                if self.man[-1] == '.':
                    self.has_dot = False
                self.man = self.man[:-1]
        elif self.state == NumberEdit.SIM:
            if len(self.sim) == 0:
                self.state = NumberEdit.MAN
            else:
                self.sim = self.sim[:-1]
        elif self.state == NumberEdit.MIX:
            if len(self.mix) == 0:
                self.state = NumberEdit.SIM
            else:
                self.mix = self.mix[:-1]
 
    def mant_digit_count(self):
        count = len(self.man)
        if self.has_dot:
            count -= 1
        return count

    def print(self):
        string = ''
        if self.is_neg:
            string += '-'
        string += str(self.man)
        if self.state == NumberEdit.EXP:
            string += ' '
            if self.is_exp_neg:
                string += '-'
            string += self.exp
        elif self.state == NumberEdit.SIM:
            string += '/' + self.sim
        elif self.state == NumberEdit.MIX:
            string += ' ' + self.sim + '/' + self.mix
        return string

def test():
    n = NumberEdit()
    n.digit('1')
    n.dot()
    n.digit('2')
    print(n.print())
    n.ee()
    n.digit('3')
    n.digit('4')
    print(n.print())
    n.back()
    print(n.print())

test()








 
