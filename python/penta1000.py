import ctypes

from ctypes.util import find_library

import os

os.environ['LD_LIBRARY_PATH'] = os.getcwd()

# Core

def p1_new(random_seed):
    global libp1
    libp1 = ctypes.cdll.LoadLibrary('libpenta1000.so')
    # core
    libp1.p1_new.argtypes = [ctypes.c_long]
    libp1.p1_new.restype = ctypes.c_void_p
    libp1.p1_release.argtypes = [ctypes.c_void_p]
    libp1.p1_press_key.argtypes = [ctypes.c_void_p, ctypes.c_char]
    libp1.p1_get_display.argtypes = [ctypes.c_void_p]
    libp1.p1_get_display.restype = ctypes.c_char_p
    # animations
    libp1.p1_advance_frame.argtypes = [ctypes.c_void_p]
    libp1.p1_is_animating.argtypes = [ctypes.c_void_p]
    libp1.p1_is_animating.restype = ctypes.c_bool
    # serialization
    libp1.p1_get_state_size.argtypes = [ctypes.c_void_p]
    libp1.p1_get_state_size.restype = ctypes.c_long
    libp1.p1_get_state.argtypes = [ctypes.c_void_p]
    libp1.p1_get_state.restype = ctypes.c_void_p
    libp1.p1_new_from_state.argtypes = [ctypes.c_void_p]
    libp1.p1_new_from_state.restype = ctypes.c_void_p
    # logging
    libp1.p1_log_get_first_available_index.argtypes = [ctypes.c_void_p]
    libp1.p1_log_get_first_available_index.restype = ctypes.c_long
    libp1.p1_log_get_last_available_index.argtypes = [ctypes.c_void_p]
    libp1.p1_log_get_last_available_index.restype = ctypes.c_long
    libp1.p1_log_get_entry.argtypes = [ctypes.c_void_p, ctypes.c_long]
    libp1.p1_log_get_entry.restype = ctypes.c_char_p
    libp1.p1_log_clear.argtypes = [ctypes.c_void_p]
    # return new object
    return libp1.p1_new(random_seed)

def p1_release(p1):
    return libp1.p1_release(p1)

def p1_press_key(p1, key):
    return libp1.p1_press_key(p1, ord(key))

def p1_get_display(p1):
    return libp1.p1_get_display(p1).decode("ascii")

# Animation

def p1_advance_frame(p1):
    return libp1.p1_advance_frame(p1)

def p1_is_animating(p1):
    return libp1.p1_is_animating(p1)

# State

def p1_get_state_size(p1):
    return libp1.p1_get_state_size(p1)

def p1_get_state(p1):
    return libp1.p1_get_state(p1)

def p1_new_from_state(p1):
    return libp1.p1_new_from_state(p1)

# Log

def p1_log_get_first_available_index(p1):
    return libp1.p1_log_get_first_available_index(p1)

def p1_log_get_last_available_index(p1):
    return libp1.p1_log_get_last_available_index(p1)

def p1_log_get_entry(p1, index):
    return libp1.p1_log_get_entry(p1, index).decode('ascii')

def p1_log_clear(p1):
    return libp1.p1_log_clear(p1)

# Test

def test():
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

test()
