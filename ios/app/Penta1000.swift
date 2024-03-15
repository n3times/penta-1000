import Foundation

/// The Pentatronics 1000 object used to run the emulator.
///
/// This is a wrapper around the Pentatronics 1000 C library.
class Penta1000 {
    static let shared = Penta1000(filename: stateFilename)

    private static let hiscoreKey = "hiscore"
    private static let stateFilename = "penta1000.dat"

    // The C backing struct.
    private var p1: OpaquePointer

    /// Initializes a Penta1000 object. The 'randomSeed' is used by the game's pseudo random number
    /// generator.
    private init(randomSeed: Int) {
        p1 = p1_new(randomSeed)
    }

    /// Initializes a Penta1000 object from the state stored in a given file. Returns non-nil if the
    /// object was successfully initialized.
    private init(filename: String) {
        let dirURL = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
        guard let fileURL = dirURL?.appendingPathComponent(filename) else {
            self.p1 = p1_new(0)
            return
        }
        guard let fileRawData = try? Data(contentsOf: fileURL) else { 
            self.p1 = p1_new(0)
            return
        }
        var p1: OpaquePointer?

        fileRawData.withUnsafeBytes { ptr in
            if let fileRawDataPointer = ptr.baseAddress?.assumingMemoryBound(to: Int8.self) {
                p1 = p1_new_from_state_buffer(fileRawDataPointer)
            }
        }

        guard let p1 else { 
            self.p1 = p1_new(0)
            return
        }
        self.p1 = p1
    }

    deinit {
        p1_release(p1)
    }

    /// Returns the calculator display as a String of at most 12 characters.
    func display() -> String {
        return String(cString: p1_get_display(p1))
    }

    /// Should be called whenever the user presses a calculator key.
    func pressKey(c: Character) {
        p1_press_key(p1, Int8(Array(c.utf8)[0]))
    }

    // MARK: Animation.

    /// Returns true if an animation is occurring on the display.
    func isAnimating() -> Bool {
        return p1_is_animating(p1)
    }

    /// Should be called every 10ms while 'isAnimating()' is true.
    func advanceFrame() {
        return p1_advance_frame(p1)
    }

    // MARK: Logging.

    /// Returns 0 if no entry is available, otherwise >= 1.
    func firstAvailableLogEntryIndex() -> Int {
        return p1_log_get_first_available_index(p1)
    }

    /// Returns 0 if no entry is available, otherwise >= 1.
    func lastAvailableLogEntryIndex() -> Int {
        return p1_log_get_last_available_index(p1)
    }

    /// Gets the log entry of given index (>= 1, between first and last index).
    func logEntry(atIndex: Int) -> String {
        return String(cString: p1_log_get_entry(p1, atIndex))
    }

    /// Clears all log entries.
    func clearLog() {
        p1_log_clear(p1)
    }

    // MARK: State.

    /// Saves the Penta1000 object in a given file. Returns 'true' if the object was saved
    /// successfully.
    func save() -> Bool {
        let dirURL: URL? =
            FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first

        guard let fileURL = dirURL?.appendingPathComponent(Penta1000.stateFilename) else {
            return false
        }

        let p1RawData = Data(bytes: p1_get_state_buffer(p1),
                             count: p1_get_state_buffer_size(p1))

        do {
            try p1RawData.write(to: fileURL, options: .atomic)
            return true
        } catch {
            return false
        }
    }

    // MARK: HI-LO hi-score.

    /// Sets the current HI-LO hi-score.
    func setStateHiscore(hiscore: Int32) {
        p1_hiscore_set(p1, hiscore)
    }

    /// Gets the current HI-LO hi-score.
    func getStateHiscore() -> Int32 {
        return p1_hiscore_get(p1)
    }

    /// Saves hi-score into UserDefaults.
    func setSavedHiscore(hiscore: Int) {
        UserDefaults.standard.set(hiscore, forKey: Penta1000.hiscoreKey);
    }

    /// Gets hi-score from UserDefaults.
    func getSavedHiscore() -> Int {
        let hiscore = UserDefaults.standard.integer(forKey: Penta1000.hiscoreKey)
        return hiscore
    }
}
