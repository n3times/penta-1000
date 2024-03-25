//
//  ContentView.swift
//  penta1000watch Watch App
//
//  Created by Paul Novaes on 3/18/24.
//  Copyright © 2024 Paul Novaes. All rights reserved.
//

import SwiftUI

struct Penta1000View: View {
    @State var displayText = Penta1000.shared.display()

    var body: some View {
        GeometryReader { geometry in
            let spacerHeight = 5.0
            let height = geometry.size.height - spacerHeight
            let displayHeightRatio = 0.125
            let keyboardHeightRatio = 1 - displayHeightRatio

            VStack {
                DisplayView(displayString: $displayText, isWatchOS: true)
                    .frame(height: height * displayHeightRatio)
                KeyboardView(displayString: $displayText)
                    .frame(height: height * keyboardHeightRatio)
                Spacer(minLength: spacerHeight)
            }
        }
    }
}

#Preview {
    Penta1000View()
}
