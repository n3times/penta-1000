//
//  ContentView.swift
//  penta1000watch Watch App
//
//  Created by Paul Novaes on 3/18/24.
//  Copyright © 2024 Paul Novaes. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    var body: some View {
        VStack {
            DisplayView(displayString: .constant("PENTATRONICS"))
        }
        .padding()
    }
}

#Preview {
    ContentView()
}
