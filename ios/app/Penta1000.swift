import Foundation

import WatchConnectivity

/// The Pentatronics 1000 object used to run the emulator.
///
/// This is a wrapper around the Pentatronics 1000 C library.
class Penta1000: NSObject, WCSessionDelegate {
    private static let hiscoreKey = "hiscore"
    static let stateFilename = "penta1000.dat"

    /// The Penta1000 singleton.
    static let shared = Penta1000()

    // The C backing struct.
    private var p1: OpaquePointer

    private static func p1(fromFilename filename: String) -> OpaquePointer? {
        let dirURL = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
        guard let fileURL = dirURL?.appendingPathComponent(filename) else { return nil }
        guard let fileRawData = try? Data(contentsOf: fileURL) else { return nil }
        var p1: OpaquePointer?

        fileRawData.withUnsafeBytes { ptr in
            if let fileRawDataPointer = ptr.baseAddress?.assumingMemoryBound(to: Int8.self) {
                p1 = p1_new_from_state_buffer(fileRawDataPointer)
            }
        }

        guard let p1 else { return nil }
        return p1
    }

    /// Initializes a Penta1000 object. The 'randomSeed' is used by the game's pseudo random number
    /// generator.
    init(withRandomSeed randomSeed: Int) {
        p1 = p1_new(randomSeed)
    }

    /// Initializes a Penta1000 object from the state stored in a given file.
    init?(fromFilename filename: String) {
        guard let p1 = Penta1000.p1(fromFilename: filename) else { return nil }

        self.p1 = p1
    }

    /// Initializes a Penta1000 object from the standard state file.
    ///
    /// Otherwise initializes from a blank state.
    override init() {
        self.p1 = Penta1000.p1(
            fromFilename: Penta1000.stateFilename) ?? p1_new(Int.random(in: 0...1_000_000))

        super.init()

        if WCSession.isSupported() {
            let session = WCSession.default
            session.delegate = self
            session.activate()
        }

        synchronizeHiscore()
    }

    deinit {
        p1_release(p1)
    }

    /// Returns the calculator display as a String.
    func display() -> String {
        return String(cString: p1_get_display(p1))
    }

    /// Should be called whenever the user presses a calculator key.
    func pressKey(_ c: Character) {
        if let c = c.asciiValue {
            p1_press_key(p1, Int8(c))
        }
    }

    // MARK: Animation.

    /// Returns true if an animation is occurring on the display.
    func isAnimating() -> Bool {
        return p1_is_animating(p1)
    }

    /// Should be called every 10 ms while 'isAnimating()' is true.
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

    /// Saves the Penta1000 object in the file syatem. Returns 'true' if the object was saved
    /// successfully.
    func save() -> Bool {
        let dirURL = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first

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

    func synchronizeHiscore(remoteHiscore: Int? = nil) {
        let savedHiscore = getSavedHiscore()
        let stateHiscore = getStateHiscore()
        let newHiscore = remoteHiscore ?? 0
        let maxScore = max(savedHiscore, Int(stateHiscore), newHiscore)

        if maxScore > savedHiscore { setSavedHiscore(hiscore: maxScore)}
        if maxScore > stateHiscore { setStateHiscore(hiscore: Int32(maxScore))}

        if remoteHiscore == nil {
            do {
                try WCSession.default.updateApplicationContext(["hiscore": getStateHiscore()])
            } catch {
                print("Error sending high score: \(error.localizedDescription)")
            }
            return
        }
    }

    // MARK: WCSessionDelegate

    func session(_ session: WCSession, activationDidCompleteWith activationState: WCSessionActivationState, error: (any Error)?) {

    }

    #if os(iOS)
    func sessionDidBecomeInactive(_ session: WCSession) {

    }

    func sessionDidDeactivate(_ session: WCSession) {

    }
    #endif

    func session(_ session: WCSession, didReceiveApplicationContext applicationContext: [String : Any]) {
        if let hiscore = applicationContext["hiscore"] {
            synchronizeHiscore(remoteHiscore: hiscore as? Int)
        }
    }
}
