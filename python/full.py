# Pentatronics 1000 emulator.

from penta1000_engine import *
from tkinter import *

class App(Frame):
    def __init__(self):
        Frame.__init__(self)
        self.master.title("Pentatronics 1000")
        self['bg'] = 'black'
        self.pack(expand=YES, fill=BOTH)

        # Place display at the top.
        self.display = StringVar()
        displayEntry = Entry(self, textvariable=self.display, justify='right',
                             font='courier 35 bold', bg='black', fg='red',
                             width=13, borderwidth=0, highlightthickness=0)
        displayEntry.grid(row=0, column=0, columnspan=4, ipady=20, padx=30)
        self.display.set(p1_get_display(p1))

        # Below the display, place the 20 calculator buttons in a grid.
        keys = ["?~%C", "789/", "456*", "123+", "0.-="]
        for row in range(5):
            for col in range(4):
                key = keys[row][col]
                button = Button(self, text=key, font='courier 30 bold')
                button['command'] = lambda k=key:self.press_key(k)
                button.grid(row=row+1, column=col, sticky='ew', padx=2, pady=2,
                            ipady=10)

    # Called whenever the user presses a calculator key.
    def press_key(self, key):
        # Translate displayed key to key understandable by the engine.
        if key == '?': key = 'g'
        if key == 'C': key = 'c'
        # Report key press to engine.
        was_animating = p1_is_animating(p1)
        p1_press_key(p1, key)
        # Update display.
        self.display.set(p1_get_display(p1))
        # Animate
        is_animating = p1_is_animating(p1)
        if is_animating and not was_animating:
            self.after(10, self.animate)

    def animate(self):
        if p1_is_animating(p1):
            p1_advance_frame(p1)
            self.display.set(p1_get_display(p1));
        if p1_is_animating(p1):
            self.after(10, self.animate)

if __name__ == '__main__':
    # Initialize the engine that drives the emulation of the Pentatronics 1000.
    p1 = p1_new(0)
    app = App()
    app.mainloop()
