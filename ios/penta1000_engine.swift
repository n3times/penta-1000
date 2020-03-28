//
//  penta1000_engine.swift
//  penta1000
//
//  Created by Paul Novaes on 3/27/20.
//  Copyright © 2020 Paul Novaes. All rights reserved.
//

import Foundation

// Core.

func engine_new(seed: CLong) -> OpaquePointer? {
    return p1_new(seed)
}

func engine_release(p1: OpaquePointer?) {
    p1_release(p1)
}

func engine_get_display(p1: OpaquePointer?) -> String {
    return String(cString: p1_get_display(p1))
}

func engine_press_key(p1: OpaquePointer?, c: Character) {
    p1_press_key(p1, Int8(Array(c.utf8)[0]))
}

// Animation.

func engine_is_animating(p1: OpaquePointer?) -> Bool {
    return p1_is_animating(p1)
}

func engine_advance_frame(p1: OpaquePointer?) {
    return p1_advance_frame(p1)
}

// Logging.

func engine_log_get_first_available_index(p1: OpaquePointer?) -> CLong {
    return p1_log_get_first_available_index(p1)
}

func engine_log_get_last_available_index(p1: OpaquePointer?) -> CLong {
    return p1_log_get_last_available_index(p1)
}

func engine_log_get_entry(p1: OpaquePointer?, index: CLong) -> String {
    return String(cString: p1_log_get_entry(p1, index))
}

func engine_log_clear(p1: OpaquePointer?) {
    p1_log_clear(p1)
}

// State.

func engine_get_state_size(p1: OpaquePointer?) -> CLong {
    return p1_get_state_size(p1)
}

func engine_get_state(p1: OpaquePointer?) -> UnsafeMutablePointer<Int8> {
    return p1_get_state(p1)
}

func engine_release_state(state: UnsafeMutablePointer<Int8>) {
    p1_release_state(state)
}

func engine_new_from_state(state: UnsafePointer<Int8>) -> OpaquePointer? {
    return p1_new_from_state(state)
}