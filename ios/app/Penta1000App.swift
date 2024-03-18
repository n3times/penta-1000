import SwiftUI

@main
struct Penta1000App: App {
    @Environment(\.scenePhase) private var scenePhase

    private func resetState() {
        do {
            let dirURL = FileManager.default.urls(for: .documentDirectory,
                                                  in: .userDomainMask).first
            if let fileURL = dirURL?.appendingPathComponent(Penta1000.stateFilename) {
                if FileManager.default.fileExists(atPath: fileURL.path) {
                    try FileManager.default.removeItem(at: fileURL)
                }
            }
        } catch let error as NSError {
            print("Couldn't remove state file: \(error.domain)")
        }
        UserDefaults.standard.set(0, forKey: "hiscore")
    }

    init() {
        if CommandLine.arguments.contains("--reset") {
            resetState()
        }
    }

    var body: some Scene {
        WindowGroup {
            Penta1000View()
        }
        .onChange(of: scenePhase) { newScenePhase in
            if newScenePhase == .background {
                _ = Penta1000.shared.save()
            }
        }
    }
}
