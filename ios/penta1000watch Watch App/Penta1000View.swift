//
//  ContentView.swift
//  penta1000watch Watch App
//
//  Created by Paul Novaes on 3/18/24.
//  Copyright © 2024 Paul Novaes. All rights reserved.
//

import SwiftUI

struct Penta1000View: View {
    @State var displayText = "PENTATRONICS"

    var body: some View {
        GeometryReader { geometry in
            let height = geometry.size.height
            let calcHeightRatio = 0.125
            let keyboardHeightRatio = 1 - calcHeightRatio

            VStack {
                DisplayView(displayString: $displayText)
                    .frame(height: height * calcHeightRatio)
                KeyboardView(displayString: $displayText)
                    .frame(height: height * keyboardHeightRatio)
            }
        }
    }
}

#Preview {
    Penta1000View()
}
