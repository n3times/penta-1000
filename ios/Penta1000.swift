//
//  Penta1000.swift
//  penta1000
//
//  Created by Paul Novaes on 3/27/20.
//  Copyright © 2020 Paul Novaes. All rights reserved.
//

import Foundation

class Penta1000 {
    let p1: OpaquePointer

    init(seed: CLong) {
        p1 = p1_new(seed)
    }

    init(state: UnsafePointer<Int8>) {
        p1 = p1_new_from_state(state)
    }

    deinit {
        p1_release(p1)
    }

    func display() -> String {
        return String(cString: p1_get_display(p1))
    }

    func pressKey(c: Character) {
        p1_press_key(p1, Int8(Array(c.utf8)[0]))
    }

    // Animation.

    func isAnimating() -> Bool {
        return p1_is_animating(p1)
    }

    func advanceFrame() {
        return p1_advance_frame(p1)
    }

    // Logging.

    func firstAvailableLogEntryIndex() -> CLong {
        return p1_log_get_first_available_index(p1)
    }

    func lastAvailableLogEntryIndex() -> CLong {
        return p1_log_get_last_available_index(p1)
    }

    func logEntry(atIndex: CLong) -> String {
        return String(cString: p1_log_get_entry(p1, atIndex))
    }

    func clearLog() {
        p1_log_clear(p1)
    }

    // State.

    func state() -> Penta1000State {
        return Penta1000State(p1: p1)
    }

    func releaseState(state: UnsafeMutablePointer<Int8>) {
        p1_release_state(state)
    }
}

class Penta1000State {
    public var size: CLong
    public var state: UnsafeMutablePointer<Int8>

    init(p1: OpaquePointer) {
        state = p1_get_state(p1)
        size = p1_get_state_size(p1)
    }

    deinit {
        p1_release_state(state)
    }
}
