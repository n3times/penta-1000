//
//  KeyboardView.swift
//  penta1000watch Watch App
//
//  Created by Paul Novaes on 3/19/24.
//  Copyright © 2024 Paul Novaes. All rights reserved.
//

import SwiftUI
import WatchKit

private struct PentaButton: View {
    let key: Character
    let width: CGFloat
    let height: CGFloat
    let penta1000 = Penta1000.shared
    @Binding var displayText: String

    private var background: Color {
        switch key {
        case "h": return .blue
        case "c": return .orange
        default: return .white
        }
    }

    private var label: String {
        let keyToLabelMap: [Character: String] = [".": "\u{00B7}",
                                                  "*": "\u{00D7}",
                                                  "h": "?",
                                                  "c": "C",
                                                  "~": "\u{00B1}",
                                                  "/": "\u{00F7}"]

        return keyToLabelMap[key] ?? String(key)
    }

    init(key: Character, width: CGFloat, height: CGFloat, displayText: Binding<String>) {
        self.key = key
        self.width = width
        self.height = height
        self._displayText = displayText
    }

    private func press(key: Character) {
        WKInterfaceDevice.current().play(WKHapticType.click)

        while penta1000.isAnimating() {
            penta1000.advanceFrame()
        }
        penta1000.pressKey(key)
        while penta1000.isAnimating() {
            penta1000.advanceFrame()
        }
        displayText = penta1000.display()
    }

    var body: some View {
        Button(action: { press(key: key) }) {
            Text(label)
                .foregroundColor(.black)
        }
        .buttonStyle(PlainButtonStyle())
        .frame(width: width, height: height)
        .contentShape(Rectangle())
        .background(background)
        .cornerRadius(8)
    }
}

#Preview {
    PentaButton(key:"h", width: 80, height: 25, displayText: .constant("PENTATRONICS"))
}

struct KeyboardView: View {
    @Binding var displayString: String

    private static let rowCount = 5
    private static let columnCount = 4
    private static let keys = ["h~%c", "789/", "456*", "123+", "0.-="]

    var body: some View {
        GeometryReader { geometry in
            let height = geometry.size.height
            let spacing = CGFloat(3)
            let buttonHeight = (height - (5 - 1) * spacing) / 5

            let width = geometry.size.width
            let buttonWidth = (width - (4 - 1) * spacing) / 4


            VStack(spacing: spacing) {
                ForEach(0..<KeyboardView.rowCount, id: \.self) { rowIndex in
                    HStack(spacing: spacing) {
                        ForEach(0..<KeyboardView.columnCount, id: \.self) { columnIndex in
                            let row = KeyboardView.keys[rowIndex]
                            let keyIndex = row.index(row.startIndex, offsetBy: columnIndex)
                            PentaButton(key: row[keyIndex], 
                                        width: buttonWidth,
                                        height: buttonHeight,
                                        displayText: $displayString)
                        }
                    }
                }
            }
        }
    }
}

#Preview {
    KeyboardView(displayString: .constant("PENTATRONICS"))
}
