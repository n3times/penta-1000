//
//  ContentView.swift
//  penta1000watch Watch App
//
//  Created by Paul Novaes on 3/18/24.
//  Copyright © 2024 Paul Novaes. All rights reserved.
//

import SwiftUI

struct Penta1000View: View {
    var body: some View {
        GeometryReader { geometry in
            let height = geometry.size.height
            
            VStack {
                DisplayView(displayString: .constant("PENTATRONICS"))
                    .frame(minHeight: height / 6, maxHeight: .infinity)
                KeyboardView()
                    .frame(minHeight: 5 * height / 6, maxHeight: .infinity)
            }
            .padding()
        }
    }
}

#Preview {
    Penta1000View()
}
