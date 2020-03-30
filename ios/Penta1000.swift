import Foundation

// The Pentatronics 1000 object used to run an emulator.
class Penta1000 {
    let p1: OpaquePointer

    // Initializes a Penta1000 object. The 'randomSeed' is used by the game's pseudo random
    // number generator.
    init(randomSeed: Int) {
        p1 = p1_new(randomSeed)
    }

    // Initializes a Penta1000 object from a raw buffer.
    init(rawBuffer: UnsafePointer<Int8>) {
        p1 = p1_new_from_state_buffer(rawBuffer)
    }

    deinit {
        p1_release(p1)
    }

    // Returns the calculator display.
    func display() -> String {
        return String(cString: p1_get_display(p1))
    }

    // Should be called whenever the user presses a calculator key.
    func pressKey(c: Character) {
        p1_press_key(p1, Int8(Array(c.utf8)[0]))
    }

    // Animation.

    func isAnimating() -> Bool {
        return p1_is_animating(p1)
    }

    // Should be called every 10ms while 'isAnimating()' is true.
    func advanceFrame() {
        return p1_advance_frame(p1)
    }

    // Logging.

    // Returns 0 if no entry is available, otherwise >= 1.
    func firstAvailableLogEntryIndex() -> Int {
        return p1_log_get_first_available_index(p1)
    }

    // Returns 0 if no entry is available, otherwise >= 1.
    func lastAvailableLogEntryIndex() -> Int {
        return p1_log_get_last_available_index(p1)
    }

    // Gets the log entry of given index (>= 1, between first and last index).
    func logEntry(atIndex: Int) -> String {
        return String(cString: p1_log_get_entry(p1, atIndex))
    }

    // Clears all log entries.
    func clearLog() {
        p1_log_clear(p1)
    }

    // State.

    // The raw representation of the Penta1000 object.
    func raw() -> Penta1000Raw {
        return Penta1000Raw(p1: p1)
    }
}

// The underlying bytes that represent a Penta1000 object. These bytes can be saved to the
// file system and used later to reconstruct the Penta1000 object with 'init(rawBuffer:)'.
class Penta1000Raw {
    public var buffer: UnsafeMutablePointer<Int8>
    public var bufferSize: Int

    init(p1: OpaquePointer) {
        buffer = p1_get_state_buffer(p1)
        bufferSize = p1_get_state_buffer_size(p1)
    }

    deinit {
        p1_release_state_buffer(buffer)
    }
}
