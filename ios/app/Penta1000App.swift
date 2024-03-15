import SwiftUI

@main
struct Penta1000App: App {
    @Environment(\.scenePhase) private var scenePhase

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
