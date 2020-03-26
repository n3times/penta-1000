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

        # Place log to the right.
        self.log = Text(self, width=30)
        log = self.log
        log.grid(row=0, column=4, rowspan=6)
        log.tag_configure('operation', font=('Verdana', 14))
        log.tag_configure('result', font=('Verdana', 14, 'bold'))
        firstIndex = p1_log_get_first_available_index(p1)
        lastIndex = p1_log_get_last_available_index(p1)
        self.last_logged_entry_index = lastIndex
        if firstIndex > 0:
            for i in range(firstIndex, lastIndex + 1):
                entry = p1_log_get_entry(p1, i).split('=')
                if i > firstIndex: self.log.insert(END,'\n\n')
                log.insert(END, entry[0], 'operation')
                log.insert(END,'\n')
                log.insert(END, entry[1], 'result')
                log.see(END)

        if p1_is_animating(p1):
            self.after(10, self.animate)

        self.master.protocol("WM_DELETE_WINDOW", self.on_closing)

    # Called when the user closes the app.
    def on_closing(self):
        # Save state.
        file = open('penta1000.dat', 'wb')
        size = p1_get_state_size(p1)
        state = p1_get_state(p1)
        data = bytearray(ctypes.string_at(state, size))
        file.write(data)
        p1_release_state(state);
        file.close()
        app.master.destroy()

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
        # Update log.
        lastIndex = p1_log_get_last_available_index(p1)
        if lastIndex > self.last_logged_entry_index:
            if self.last_logged_entry_index > 0: self.log.insert(END,'\n\n')
            self.last_logged_entry_index = lastIndex
            entry = p1_log_get_entry(p1, lastIndex).split('=')
            log = self.log
            log.insert(END, entry[0], 'operation')
            log.insert(END,'\n')
            log.insert(END, entry[1], 'result')
            log.see(END)
        # Animate
        is_animating = p1_is_animating(p1)
        if is_animating and not was_animating:
            self.after(10, self.animate)

    # Called when animating.
    def animate(self):
        if p1_is_animating(p1):
            p1_advance_frame(p1)
            self.display.set(p1_get_display(p1));
        if p1_is_animating(p1):
            self.after(10, self.animate)

if __name__ == '__main__':
    # Use the previous state, if available, and initialize the engine that
    # drives the emulation of the Pentatronics 1000.
    try:
        with open('penta1000.dat', 'rb') as file:
            p1 = p1_new_from_state(file.read())
    except IOError:
        p1 = p1_new(0)
    app = App()
    app.mainloop()
