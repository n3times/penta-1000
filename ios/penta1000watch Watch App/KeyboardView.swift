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
    let onButtonTapped: (Character) -> Void
    @State private var isTapped = false

    private var background: Color {
        var color: Color
        switch key {
        case "h": color = .blue
        case "c": color = .orange
        default: color = .white
        }
        return isTapped ? color.opacity(0.6) : color
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

    init(key: Character, width: CGFloat, height: CGFloat, displayText: Binding<String>,
         onButtonTapped: @escaping (Character) -> Void
    ) {
        self.key = key
        self.width = width
        self.height = height
        self.onButtonTapped = onButtonTapped
    }

    var body: some View {
        Text(label)
            .foregroundColor(.black)
            .frame(width: width, height: height)
            .background(background)
            .cornerRadius(8)
            .gesture(
                DragGesture(minimumDistance: 0, coordinateSpace: .local)
                    .onChanged { _ in
                        if isTapped { return }
                        isTapped = true
                        onButtonTapped(key)
                    }
                    .onEnded { _ in
                        isTapped = false
                    }
            )
    }
}

#Preview {
    PentaButton(key:"h", width: 80, height: 25, displayText: .constant("?")) { key in
        print(key)
    }
}

struct KeyboardView: View {
    @Binding var displayString: String

    private static let rowCount = 5
    private static let columnCount = 4
    private static let keys = ["h~%c", "789/", "456*", "123+", "0.-="]

    private let penta1000 = Penta1000.shared

    private func runDisplayAnimationLoop() {
        Timer.scheduledTimer(withTimeInterval: 0.05, repeats: true) { timer in
            let oldHiscore = penta1000.getStateHiscore()

            // Advance up to 5 frames at a time, for performance reasons.
            for _ in 0..<5 {
                penta1000.advanceFrame()
                if !penta1000.isAnimating() {
                    break
                }
            }

            let newHiscore = penta1000.getStateHiscore()
            if newHiscore > oldHiscore {
                penta1000.synchronizeHiscore()
            }

            displayString = penta1000.display()

            if !penta1000.isAnimating() {
                timer.invalidate()
            }
        }
    }

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
                                        displayText: $displayString) { key in
                                WKInterfaceDevice.current().play(WKHapticType.click)

                                let wasAnimating = penta1000.isAnimating()
                                let oldHiscore = penta1000.getStateHiscore()
                                penta1000.pressKey(key)
                                let newHiscore = penta1000.getStateHiscore()
                                if newHiscore > oldHiscore {
                                    penta1000.synchronizeHiscore()
                                }
                                displayString = penta1000.display()
                                let isAnimating = penta1000.isAnimating()
                                if !wasAnimating && isAnimating {
                                    runDisplayAnimationLoop()
                                }
                            }
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
