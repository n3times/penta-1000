import Foundation

// The Pentatronics 1000 object used to run the emulator.
class Penta1000 {
    let p1: OpaquePointer?

    // Initializes a Penta1000 object. The 'randomSeed' is used by the game's pseudo random
    // number generator.
    init(randomSeed: Int) {
        p1 = p1_new(randomSeed)
    }

    // Initializes a Penta1000 object from the state stored in a given file. Returns
    // non-nil if the object was successfully initialized.
    init?(filename: String) {
        var fileRawBuffer: UnsafePointer<Int8> = UnsafePointer<Int8>(bitPattern: 1)!
        var initialized = false
        let dirURL: URL? =
            FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
        let fileURL: URL? = dirURL?.appendingPathComponent(filename)
        if (fileURL != nil) {
            var fileRawData: Data? = nil
            do {
                try fileRawData = Data(contentsOf: fileURL!)
            } catch {
            }
            if (fileRawData != nil) {
                fileRawBuffer = fileRawData!.withUnsafeBytes({
                    (ptr) -> UnsafePointer<Int8> in
                    return ptr.baseAddress!.assumingMemoryBound(to: Int8.self)
                })
                initialized = true
            }
        }
        if (initialized) {
            p1 = p1_new_from_state_buffer(fileRawBuffer)
            if (p1 == nil) { return nil; }
        } else {
            return nil
        }
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

    // Saves the Penta1000 object in a given file. Returns 'true' if the object was saved
    // successfully.
    func save(filename: String) -> Bool {
        let p1Raw: Penta1000Raw = self.raw()
        let p1RawData = Data(bytes: p1Raw.buffer, count: p1Raw.bufferSize)
        let dirURL: URL? =
            FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
        let fileURL: URL? = dirURL?.appendingPathComponent("penta1000.dat")
        var saved = false

        if (fileURL != nil) {
            do {
                try p1RawData.write(to: fileURL!, options: .atomic)
                saved = true
            } catch {
                // Nothing
            }
        }
        return saved
    }

    // The raw representation of the Penta1000 object.
    func raw() -> Penta1000Raw {
        return Penta1000Raw(p1: p1!)
    }

    // HI-LO hi-score.

    // Sets the current HI-LO hi-score.
    func setStateHiscore(hiscore: Int32) {
        p1_hiscore_set(p1, hiscore)
    }

    // Gets the current HI-LO hi-score.
    func getStateHiscore() -> Int32 {
        return p1_hiscore_get(p1)
    }

    // Saves hi-score into UserDefaults.
    func setSavedHiscore(hiscore: Int) {
        UserDefaults.standard.set(hiscore, forKey: "hiscore");
    }

    // Gets hi-score from UserDefaults.
    func getSavedHiscore() -> Int {
        let hiscore = UserDefaults.standard.integer(forKey: "hiscore")
        return hiscore
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
