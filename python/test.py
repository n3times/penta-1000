from penta1000 import *

def example():
    # core
    p1 = p1_new(0)
    print(p1_get_display(p1))
    p1_press_key(p1,'1')
    p1_press_key(p1,'+')
    p1_press_key(p1,'1')
    p1_press_key(p1,'=')
    p1_press_key(p1,'/')
    p1_press_key(p1,'0')
    p1_press_key(p1,'=')
    print(p1_get_display(p1))
    p1_press_key(p1,'g')
    print(p1_get_display(p1))
    # animation
    p1_advance_frame(p1)
    print(p1_get_display(p1))
    # logging
    first = p1_log_get_first_available_index(p1)
    last = p1_log_get_last_available_index(p1)
    print(first, last, p1_log_get_entry(p1, first))
    print(first, last, p1_log_get_entry(p1, last))
    p1_log_clear(p1)
    first = p1_log_get_first_available_index(p1)
    last = p1_log_get_last_available_index(p1)
    print(first, last)
    # save state
    file = open('penta1000.dat', 'wb')
    size = p1_get_state_size(p1)
    state = p1_get_state(p1)
    data = bytearray(ctypes.string_at(state, size))
    file.write(data)
    file.close()
    # read state
    file = open('penta1000.dat', 'rb')
    p1_from_state = p1_new_from_state(file.read())
    file.close()
    print(p1_get_display(p1_from_state))
    p1_release(p1)
    p1_release(p1_from_state)

example()
