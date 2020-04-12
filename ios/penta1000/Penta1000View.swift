import SwiftUI

struct Penta1000View: View {
    private let penta1000: Penta1000
    @State private var displayText: String = "READY"
    @State private var isTapped = false

    init(penta1000: Penta1000) {
        self.penta1000 = penta1000
    }

    // Returns the key at a given location, or nil if there is no such a key.
    private func getCalculatorKey(standardizedLocation: CGPoint) -> Character? {
        // Top left corner of top left key ("?").
        let x0 = 37.0
        let y0 = 313.0

        // Separation between the top left corners of consecutive keys.
        let dx = 78.5
        let dy = 67.0

        // Dimensions of each key.
        let w = 65.0
        let h = 51.0

        let x = Double(standardizedLocation.x) - x0
        let y = Double(standardizedLocation.y) - y0

        var i = (x + (dx - w)/2) / dx
        var j = (y + (dy - h)/2) / dy

        if (i >= -0.2 && j >= -0.2 && i < 4.2 && j < 5.2) {
            if i < 0 { i = 0 }
            if j < 0 { j = 0 }
            if i >= 4 { i = 3 }
            if (j >= 5) { j = 4 }
            let keys = ["g~%c", "789/", "456*", "123+", "0.-="]
            let index = keys[Int(j)].index(keys[Int(j)].startIndex, offsetBy: Int(i))
            return keys[Int(j)][index]
        } else {
            return nil
        }
    }

    private func appeared() {
        displayText = penta1000.display()
        if self.penta1000.isAnimating() {
            Timer.scheduledTimer(withTimeInterval: 0.02, repeats: true, block: { timer in
                self.penta1000.advanceFrame()
                if (self.penta1000.isAnimating()) { self.penta1000.advanceFrame() }
                self.displayText = self.penta1000.display()

                if !self.penta1000.isAnimating() {
                    timer.invalidate()
                }
            })
        }
    }

    func getView(_ metrics: GeometryProxy) -> some View {
        let standardCalcWidth = 375.0
        let standardCalcHeight = 647.0
        let standardDisplayHeight = 96.0
        let standardDisplayOffsetY = 197.0

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
            Color(red: 16/255, green: 16/255, blue: 16/255).edgesIgnoringSafeArea(.all)
            Image("penta1000")
                .resizable()
                .frame(width: CGFloat(calcWidth), height: CGFloat(calcHeight), alignment: .center)
                .onAppear(perform: self.appeared)
                .gesture(
                    DragGesture(minimumDistance: 0, coordinateSpace: .local)
                        .onChanged {
                            if (self.isTapped) { return; }
                            self.isTapped = true;
                            let standardizedLocation = CGPoint(x: $0.location.x / CGFloat(scaleFactor),
                                                               y: $0.location.y / CGFloat(scaleFactor))
                            let c =
                                self.getCalculatorKey(standardizedLocation: standardizedLocation)
                            if c != nil {
                                let wasAnimating = self.penta1000.isAnimating()
                                self.penta1000.pressKey(c: c!)
                                let isAnimating = self.penta1000.isAnimating()
                                self.displayText = self.penta1000.display()
                                if !wasAnimating && isAnimating {
                                    Timer.scheduledTimer(withTimeInterval: 0.02, repeats: true) { timer in
                                        self.penta1000.advanceFrame()
                                        if (self.penta1000.isAnimating()) {
                                            self.penta1000.advanceFrame()
                                        }
                                        self.displayText = self.penta1000.display()

                                        if !self.penta1000.isAnimating() {
                                            timer.invalidate()
                                        }
                                    }
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
        }
    }

    var body: some View {
      GeometryReader { metrics in
        self.getView(metrics)
      }
    }
}

struct Penta1000View_Previews: PreviewProvider {
    static var previews: some View {
        Penta1000View(penta1000: Penta1000(randomSeed: 0))
    }
}
