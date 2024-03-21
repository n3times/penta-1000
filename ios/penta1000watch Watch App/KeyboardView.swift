//
//  KeyboardView.swift
//  penta1000watch Watch App
//
//  Created by Paul Novaes on 3/19/24.
//  Copyright © 2024 Paul Novaes. All rights reserved.
//

import SwiftUI

private struct PentaButton: View {
    let title: String
    let background: Color
    let width: CGFloat
    let height: CGFloat
    let action: () -> Void

    init(_ title: String, background: Color = .white, width: CGFloat, height: CGFloat, action: @escaping () -> Void) {
        self.title = title
        self.background = background
        self.width = width
        self.height = height
        self.action = action
    }

    var body: some View {
        Button(action: action) {
            Text(title)
                .foregroundColor(.black)
        }
        .buttonStyle(PlainButtonStyle())
        .frame(width: width, height: height)
        .background(background)
        .cornerRadius(8)
    }
}

struct KeyboardView: View {
    var body: some View {
        GeometryReader { geometry in
            let height = geometry.size.height
            let spacing = CGFloat(4)
            let buttonHeight = (height - 4 * spacing) / 5

            let width = geometry.size.width
            let buttonWidth = (width - 3 * spacing) / 4

            VStack(spacing: spacing) {
                HStack(spacing: 4) {
                    PentaButton("?", background: .blue, width: buttonWidth, height: buttonHeight) { print("?") }
                    PentaButton("\u{00B1}", width: buttonWidth, height: buttonHeight) { print("\u{00B1}") }
                    PentaButton("%", width: buttonWidth, height: buttonHeight) { print("%") }
                    PentaButton("C", background: .orange, width: buttonWidth, height: buttonHeight) { print("C") }
                }
                HStack(spacing: 4) {
                    PentaButton("7", width: buttonWidth, height: buttonHeight) { print("7") }
                    PentaButton("8", width: buttonWidth, height: buttonHeight) { print("8") }
                    PentaButton("9", width: buttonWidth, height: buttonHeight) { print("9") }
                    PentaButton("\u{00F7}", width: buttonWidth, height: buttonHeight) { print("\u{00F7}") }
                }
                HStack(spacing: 4) {
                    PentaButton("4", width: buttonWidth, height: buttonHeight) { print("4") }
                    PentaButton("5", width: buttonWidth, height: buttonHeight) { print("5") }
                    PentaButton("6", width: buttonWidth, height: buttonHeight) { print("6") }
                    PentaButton("\u{00D7}", width: buttonWidth, height: buttonHeight) { print("\u{00D7}") }
                }
                HStack(spacing: 4) {
                    PentaButton("1", width: buttonWidth, height: buttonHeight) { print("1") }
                    PentaButton("2", width: buttonWidth, height: buttonHeight) { print("2") }
                    PentaButton("3", width: buttonWidth, height: buttonHeight) { print("3") }
                    PentaButton("+", width: buttonWidth, height: buttonHeight) { print("+") }
                }
                HStack(spacing: 4) {
                    PentaButton("0", width: buttonWidth, height: buttonHeight) { print("0") }
                    PentaButton("\u{00B7}", width: buttonWidth, height: buttonHeight) { print("\u{00B7}") }
                    PentaButton("-", width: buttonWidth, height: buttonHeight) { print("-") }
                    PentaButton("=", width: buttonWidth, height: buttonHeight) { print("=") }
                }
            }
        }
    }
}

