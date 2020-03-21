# Simple Pentatronics 1000 emulator.

from penta1000 import *
from tkinter import *

class App(Frame):
    def __init__(self):
        Frame.__init__(self)
        self.pack(expand=YES, fill=BOTH)
        self.master.title("Pentatronics 1000")
        self['bg'] = 'black'

        # Place display at the top.
        display = StringVar()
        displayEntry = Entry(self, textvariable=display, justify='right',
                             font='courier 35 bold', bg='black', fg='red',
                             width=13, borderwidth=0, highlightthickness=0)
        displayEntry.grid(row=0, column=0, columnspan=4, ipady=20, padx=30)

        # Set display to initial value.
        display.set(p1_get_display(p1))

        # Below the display, place the 20 caclculator buttons in a grid.
        keys = ["g~%c", "789/", "456*", "123-", "0.=+"]
        for row in range(5):
            for col in range(4):
                key = keys[row][col]
                button = Button(self, text=key, font='courier 30 bold')
                bg = 'white'
                if key == 'c': bg = 'yellow'
                if key == 'g': bg = 'gray'
                button['highlightbackground'] = bg
                button['command'] = lambda d=display, k=key: self.calc(d, k)
                button.grid(row=row+1, column=col, sticky='ew', padx=2, pady=2,
                            ipady=10)

    def calc(self, display, key):
        # Report key press to engine.
        p1_press_key(p1, key)
        # Ignore animations.
        for i in range(500):
            if p1_is_animating(p1):
                p1_advance_frame(p1)
        # Update display.
        display.set(p1_get_display(p1))

if __name__ == '__main__':
    # Initialize the engine (p1) that emulates a Pentatronics 1000.
    p1 = p1_new(0)
    App().mainloop()
