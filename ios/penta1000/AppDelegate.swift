import UIKit

@UIApplicationMain
class AppDelegate: UIResponder, UIApplicationDelegate {



    func application(_ application: UIApplication, didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
        if ProcessInfo.processInfo.arguments.contains("--Reset") {
            // Clear state.
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
        return true
    }

    // MARK: UISceneSession Lifecycle

    func application(_ application: UIApplication, configurationForConnecting connectingSceneSession: UISceneSession, options: UIScene.ConnectionOptions) -> UISceneConfiguration {
        // Called when a new scene session is being created.
        // Use this method to select a configuration to create the new scene with.
        return UISceneConfiguration(name: "Default Configuration", sessionRole: connectingSceneSession.role)
    }

    func application(_ application: UIApplication, didDiscardSceneSessions sceneSessions: Set<UISceneSession>) {
        // Called when the user discards a scene session.
        // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
        // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
    }


}

