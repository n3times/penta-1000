import SwiftUI
import AudioToolbox

/// The keyboard view.
///
/// It also listens to key presses events and runs the animation loop.
struct KeyboardView: View {
    let penta1000: Penta1000
    @Binding var displayText: String
    @State private var isTapped = false

    private static func keyHit(atStandardizedLocation location: CGPoint) -> Character? {
        // Top left corner of top left key ("?").
        let x0 = 28.0
        let y0 = 309.0

        // Dimensions of each key.
        let w = 68.0
        let h = 51.0

        // Separation between keys.
        let interX = 16.5
        let interY = 16.0

        let x = Double(location.x) - x0
        let y = Double(location.y) - y0

        var i = (x + interX / 2) / (w + interX)
        var j = (y + interY / 2) / (h + interY)

        // If the key press is a near miss, choose the closest key.
        if i >= -0.2 && j >= -0.15 && i < 4.2 && j < 5.15 {
            if i < 0 { i = 0 }
            if j < 0 { j = 0 }
            if i >= 4 { i = 3 }
            if j >= 5 { j = 4 }
            let col = Int(i)
            let row = Int(j)
            let keys = ["h~%c", "789/", "456*", "123+", "0.-="]
            let index = keys[row].index(keys[row].startIndex, offsetBy: col)
            return keys[row][index]
        } else {
            return nil
        }
    }

    private func saveHiscore() {
        let stateHiscore = penta1000.getStateHiscore()
        let savedHiscore = penta1000.getSavedHiscore()
        if stateHiscore > savedHiscore {
            penta1000.setSavedHiscore(hiscore: Int(stateHiscore))
        }
    }

    private func runDisplayAnimationLoop() {
        Timer.scheduledTimer(withTimeInterval: 0.05, repeats: true) { timer in
            let oldHiscore = penta1000.getStateHiscore()

            // Advance 5 frames at a time, for performance reasons.
            for _ in 0..<5 {
                penta1000.advanceFrame()
                if !penta1000.isAnimating() {
                    break
                }
            }

            let newHiscore = penta1000.getStateHiscore()
            if newHiscore > oldHiscore { saveHiscore() }
            displayText = penta1000.display()

            if !penta1000.isAnimating() {
                timer.invalidate()
            }
        }
    }

    var body: some View {
        return GeometryReader { metrics in
            let standardCalcWidth = 375.0
            let standardCalcHeight = 647.0

            let screenWidth = Double(metrics.size.width)
            let screenHeight = Double(metrics.size.height)

            let screenAspectRatio = screenHeight / screenWidth
            let calcAspectRatio = standardCalcHeight / standardCalcWidth
            let isPortrait = screenAspectRatio >= calcAspectRatio

            let calcWidth = isPortrait ? screenWidth : screenHeight / calcAspectRatio

            let scaleFactor = calcWidth / standardCalcWidth

            return ZStack {
                Image("penta1000")
                    .resizable()
                    .gesture(
                        // To be responsive, handle key presses as soon as the user touches the screen,
                        // instead of waiting until the user lifts the finger.
                        DragGesture(minimumDistance: 0, coordinateSpace: .local)
                            .onChanged {
                                // Determine which key was hit.
                                if isTapped { return }
                                isTapped = true
                                let standardizedLocation =
                                    CGPoint(x: $0.location.x / CGFloat(scaleFactor),
                                            y: $0.location.y / CGFloat(scaleFactor))
                                let key = KeyboardView.keyHit(
                                    atStandardizedLocation: standardizedLocation)
                                guard let key else { return }

                                AudioServicesPlaySystemSound(SystemSoundID(0x450))
                                let wasAnimating = penta1000.isAnimating()
                                let oldHiscore = penta1000.getStateHiscore()
                                penta1000.pressKey(c: key)
                                let newHiscore = penta1000.getStateHiscore()
                                if newHiscore > oldHiscore { saveHiscore() }
                                let isAnimating = penta1000.isAnimating()
                                displayText = penta1000.display()
                                if !wasAnimating && isAnimating {
                                    runDisplayAnimationLoop()
                                }
                            }
                            .onEnded { _ in
                                isTapped = false
                            }
                    )
                    .onAppear {
                        if penta1000.isAnimating() {
                            runDisplayAnimationLoop()
                        }
                    }
                    .accessibility(identifier: "calculator")
                    .accessibility(label: Text(displayText))
            }        }
    }
}

struct KeyboardView_Previews: PreviewProvider {
    static var previews: some View {
        Penta1000View(penta1000: Penta1000(randomSeed: 0))
    }
}
