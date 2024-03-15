import SwiftUI

@main
struct Penta1000App: App {
    @Environment(\.scenePhase) private var scenePhase

    private func resetState() {
        do {
            let stateFilename = "penta1000.dat"
            let dirURL: URL? =
            FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
            let fileURL: URL? = dirURL?.appendingPathComponent(stateFilename)
            try FileManager.default.removeItem(at: fileURL!)
        } catch let error as NSError {
            print("Error: \(error.domain)")
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
