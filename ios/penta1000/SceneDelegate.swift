import UIKit
import SwiftUI

class SceneDelegate: UIResponder, UIWindowSceneDelegate {
    let lastVersionUsedKey = "lastVersionUsed"
    let firstVersionUsedKey = "firstVersionUsed"
    let crashKey = "crash"
    let stateFilename = "penta1000.dat"

    var window: UIWindow?
    var penta1000: Penta1000?

    func scene(_ scene: UIScene, willConnectTo session: UISceneSession, options connectionOptions: UIScene.ConnectionOptions) {
        // Use this method to optionally configure and attach the UIWindow `window` to the provided UIWindowScene `scene`.
        // If using a storyboard, the `window` property will automatically be initialized and attached to the scene.
        // This delegate does not imply the connecting scene or session are new (see `application:configurationForConnectingSceneSession` instead).

        // Keep track of the app version.
        let currentVersion = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String
        let firstVersionUsed = UserDefaults.standard.string(forKey: firstVersionUsedKey)
        let lastVersionUsed = UserDefaults.standard.string(forKey: lastVersionUsedKey)
        if currentVersion != lastVersionUsed {
            UserDefaults.standard.set(currentVersion, forKey: lastVersionUsedKey);
            if firstVersionUsed == nil {
                UserDefaults.standard.set(currentVersion, forKey: firstVersionUsedKey);
            }
        }

        // Restore the state if there was no crash and the state is compatible with the current
        // engine.
        let didCrash = UserDefaults.standard.bool(forKey: crashKey)
        UserDefaults.standard.set(true, forKey: crashKey)
        if !didCrash { penta1000 = Penta1000(filename: stateFilename) }
        if penta1000 == nil {
            penta1000 = Penta1000(randomSeed: Int.random(in: 1...1000000000))
        }

        if let windowScene = scene as? UIWindowScene {
            let window = UIWindow(windowScene: windowScene)
            let penta1000View = Penta1000View(penta1000: penta1000!)
            window.rootViewController = HostingController(rootView: penta1000View)
            self.window = window
            window.makeKeyAndVisible()
        }
    }

    func sceneDidDisconnect(_ scene: UIScene) {
        // Called as the scene is being released by the system.
        // This occurs shortly after the scene enters the background, or when its session is discarded.
        // Release any resources associated with this scene that can be re-created the next time the scene connects.
        // The scene may re-connect later, as its session was not neccessarily discarded (see `application:didDiscardSceneSessions` instead).
    }

    func sceneDidBecomeActive(_ scene: UIScene) {
        // Called when the scene has moved from an inactive state to an active state.
        // Use this method to restart any tasks that were paused (or not yet started) when the scene was inactive.
    }

    func sceneWillResignActive(_ scene: UIScene) {
        // Called when the scene will move from an active state to an inactive state.
        // This may occur due to temporary interruptions (ex. an incoming phone call).
    }

    func sceneWillEnterForeground(_ scene: UIScene) {
        // Called as the scene transitions from the background to the foreground.
        // Use this method to undo the changes made on entering the background.
    }

    func sceneDidEnterBackground(_ scene: UIScene) {
        // Called as the scene transitions from the foreground to the background.
        // Use this method to save data, release shared resources, and store enough scene-specific state information
        // to restore the scene back to its current state.
        let wasSaved = penta1000!.save(filename: stateFilename)
        if wasSaved { UserDefaults.standard.set(false, forKey: crashKey); }
    }


}

