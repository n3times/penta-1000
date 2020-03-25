# Tests the Python interface to the C engine, that is that calls from Python
# correctly call the engine and that results are propagated back.

from penta1000_engine import *

def assert_display(expected):
    actual = p1_get_display(p1)
    if actual != expected:
        print("error: expected = %s actual = %s" %(expected, actual))
    else:
        print("success:", actual)

def assert_log_interval(expected_first, expected_last):
    actual = p1_log_get_first_available_index(p1)
    expected = expected_first
    if actual != expected:
        print("error: expected = %s actual = %s" %(expected, actual))
    else:
        print("success:", actual)
    actual = p1_log_get_last_available_index(p1)
    expected = expected_last
    if actual != expected:
        print("error: expected = %s actual = %s" %(expected, actual))
    else:
        print("success:", actual)

def assert_log(index, expected):
    actual = p1_log_get_entry(p1, index)
    if actual != expected:
        print("error: expected = %s actual = %s" %(expected, actual))
    else:
        print("success:", actual)

def test():
    global p1
    p1 = p1_new(0)

    # core
    assert_display("PENTATRONICS")

    p1_press_key(p1, '1')
    p1_press_key(p1, '+')
    p1_press_key(p1, '1')
    p1_press_key(p1, '=')
    assert_display("2")

    p1_press_key(p1, '/')
    p1_press_key(p1, '0')
    p1_press_key(p1, '=')
    assert_display("DIV BY ZERO")

    p1_press_key(p1, 'g')
    assert_display("> HI-LO GAME")

    # animation
    while p1_is_animating(p1):
        p1_advance_frame(p1)
    assert_display("___         ")

    # logging
    assert_log_interval(1, 2)
    assert_log(1, "1+1=2")
    assert_log(2, "2/0=DIV BY ZERO")
    p1_log_clear(p1)
    assert_log_interval(0, 0)

    # save state
    file = open('penta1000.dat', 'wb')
    size = p1_get_state_size(p1)
    state = p1_get_state(p1)
    data = bytearray(ctypes.string_at(state, size))
    file.write(data)
    p1_release_state(state);
    file.close()

    # read state
    file = open('penta1000.dat', 'rb')
    p1_from_state = p1_new_from_state(file.read())
    assert_display("___         ")

    # clean up
    file.close()
    p1_release(p1)
    p1_release(p1_from_state)

test()
