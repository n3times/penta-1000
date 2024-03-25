//
//  penta1000watchApp.swift
//  penta1000watch Watch App
//
//  Created by Paul Novaes on 3/18/24.
//  Copyright © 2024 Paul Novaes. All rights reserved.
//

import SwiftUI

@main
struct penta1000watch_Watch_AppApp: App {
    @Environment(\.scenePhase) private var scenePhase

    var body: some Scene {
        WindowGroup {
            Penta1000View()
                .ignoresSafeArea(edges: [.bottom, .leading, .trailing])
        }
        .onChange(of: scenePhase) { oldScenePhase, newScenePhase in
            if oldScenePhase == .active && newScenePhase == .inactive {
                _ = Penta1000.shared.save()
            }
        }
    }
}
