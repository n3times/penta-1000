import ctypes

from ctypes.util import find_library

def p1_new(random_seed):
    global libp1
    libp1 = ctypes.cdll.LoadLibrary(find_library('libpenta1000.so'))
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
    # return
    return libp1.p1_new(random_seed)

def p1_press_key(p1, key):
    return libp1.p1_press_key(p1, ord(key))

def p1_get_display(p1):
    return libp1.p1_get_display(p1).decode("ascii")

def test():
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

test()
