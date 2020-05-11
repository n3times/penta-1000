import SwiftUI
import AudioToolbox

// The main view. It holds the calculator with its keyboard and display. It listens to key presses
// events and runs the animation loop.
struct Penta1000View: View {
    private let penta1000: Penta1000
    @State private var displayText: String = "READY"
    @State private var isTapped = false

    init(penta1000: Penta1000) {
        self.penta1000 = penta1000
    }

    private static func getCalculatorKey(standardizedLocation: CGPoint) -> Character? {
        // Top left corner of top left key ("?").
        let x0 = 28.0
        let y0 = 309.0

        // Dimensions of each key.
        let w = 68.0
        let h = 51.0

        // Separation between keys.
        let interX = 16.5
        let interY = 16.0

        let x = Double(standardizedLocation.x) - x0
        let y = Double(standardizedLocation.y) - y0

        var i = (x + interX / 2) / (w + interX)
        var j = (y + interY / 2) / (h + interY)

        // If the key press is a near miss, we choose the closest key.
        if (i >= -0.2 && j >= -0.15 && i < 4.2 && j < 5.15) {
            if i < 0 { i = 0 }
            if j < 0 { j = 0 }
            if i >= 4 { i = 3 }
            if (j >= 5) { j = 4 }
            let col = Int(i)
            let row = Int(j)
            let keys = ["h~%c", "789/", "456*", "123+", "0.-="]
            let index = keys[row].index(keys[row].startIndex, offsetBy: col)
            return keys[row][index]
        } else {
            return nil
        }
    }

    private func runDisplayAnimationLoop() {
        Timer.scheduledTimer(withTimeInterval: 0.05, repeats: true, block: { timer in
            // Advance 5 frames at a time, for performance reasons.
            self.penta1000.advanceFrame()
            if (self.penta1000.isAnimating()) { self.penta1000.advanceFrame() }
            if (self.penta1000.isAnimating()) { self.penta1000.advanceFrame() }
            if (self.penta1000.isAnimating()) { self.penta1000.advanceFrame() }
            if (self.penta1000.isAnimating()) { self.penta1000.advanceFrame() }
            self.displayText = self.penta1000.display()
            if !self.penta1000.isAnimating() {
                timer.invalidate()
            }
        })
    }

    func getView(_ metrics: GeometryProxy) -> some View {
        let standardCalcWidth = 375.0
        let standardCalcHeight = 647.0
        let standardDisplayHeight = 96.0
        let standardDisplayOffsetY = 195.0

        let screenWidth = Double(metrics.size.width)
        let screenHeight = Double(metrics.size.height)

        let screenAspectRatio = screenHeight / screenWidth
        let calcAspectRatio = standardCalcHeight / standardCalcWidth
        let isPortrait = screenAspectRatio >= calcAspectRatio

        let calcWidth = isPortrait ? screenWidth : screenHeight / calcAspectRatio
        let calcHeight = isPortrait ? screenWidth * calcAspectRatio : screenHeight

        let scaleFactor = calcWidth / standardCalcWidth

        let displayWidth = calcWidth
        let displayHeight = standardDisplayHeight * scaleFactor

        let displayOffsetX = CGFloat(0.0)
        let displayOffsetY =
            (standardDisplayOffsetY - (standardCalcHeight - standardDisplayHeight)/2) * scaleFactor

        return ZStack {
            Color(red: 16.0/255, green: 16.0/255, blue: 16.0/255).edgesIgnoringSafeArea(.all)
            Image("penta1000")
                .resizable()
                .frame(width: CGFloat(calcWidth), height: CGFloat(calcHeight), alignment: .center)
                .gesture(
                    // To be responsive, handle key presses as soon as the user touches the screen,
                    // instead of waiting until the user lifts the finger/stylus.
                    DragGesture(minimumDistance: 0, coordinateSpace: .local)
                        .onChanged {
                            if (self.isTapped) { return; }
                            self.isTapped = true;
                            let standardizedLocation =
                                CGPoint(x: $0.location.x / CGFloat(scaleFactor),
                                        y: $0.location.y / CGFloat(scaleFactor))
                            let c = Penta1000View.getCalculatorKey(
                                standardizedLocation: standardizedLocation)
                            if c != nil {
                                AudioServicesPlaySystemSound(SystemSoundID(0x450))
                                let wasAnimating = self.penta1000.isAnimating()
                                self.penta1000.pressKey(c: c!)
                                let isAnimating = self.penta1000.isAnimating()
                                self.displayText = self.penta1000.display()
                                if !wasAnimating && isAnimating {
                                    self.runDisplayAnimationLoop()
                                }
                            }
                        }
                        .onEnded { _ in
                            self.isTapped = false
                        }
                )
                .accessibility(identifier: "calculator")
                .accessibility(label: Text(self.displayText))
            DisplayView(self.$displayText)
                .offset(x: CGFloat(displayOffsetX), y: CGFloat(displayOffsetY))
                .frame(width: CGFloat(displayWidth),
                       height: CGFloat(displayHeight),
                       alignment: .center)
                .onAppear {
                    self.displayText = self.penta1000.display()
                    if self.penta1000.isAnimating() {
                        self.runDisplayAnimationLoop()
                    }
                }
        }
    }

    var body: some View {
        return GeometryReader { metrics in
            self.getView(metrics)
        }
    }
}

struct Penta1000View_Previews: PreviewProvider {
    static var previews: some View {
        Penta1000View(penta1000: Penta1000(randomSeed: 0))
    }
}
