import SwiftUI

struct Penta1000View: View {
    private let penta1000: Penta1000
    @State var displayText: String = "READY"

    init(penta1000: Penta1000) {
        self.penta1000 = penta1000
    }

    // Returns the key at a given location, or nil if there is no such a key.
    private func getCalculatorKey(location: CGPoint) -> Character? {
        let x = location.x
        let y = location.y

        // Top left corner of top left key ("?").
        let x0: CGFloat = 37
        let y0: CGFloat = 313

        // Separation between the top left corners of consecutive keys.
        let dx: CGFloat = 78.5
        let dy: CGFloat = 67

        // Dimensions of each key.
        let w: CGFloat = 65
        let h: CGFloat = 51

        var i: CGFloat = (x - x0 + (dx - w)/2) / dx
        var j: CGFloat = (y - y0 + (dy - h)/2) / dy

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
            Timer.scheduledTimer(withTimeInterval: 0.01, repeats: true, block: { timer in
                self.penta1000.advanceFrame()
                self.displayText = self.penta1000.display()

                if !self.penta1000.isAnimating() {
                    timer.invalidate()
                }
            })
        }
    }

    // Listens for tap events to determine if user pressed a calculator key.
    private var dragGesture: some Gesture {
        var isTapped = false
        return DragGesture(minimumDistance: 0, coordinateSpace: .local)
            .onChanged {
                if (isTapped) { return; }
                isTapped = true;
                let c = self.getCalculatorKey(location: $0.location)
                if c != nil {
                    let wasAnimating = self.penta1000.isAnimating()
                    self.penta1000.pressKey(c: c!)
                    let isAnimating = self.penta1000.isAnimating()
                    self.displayText = self.penta1000.display()
                    if !wasAnimating && isAnimating {
                        Timer.scheduledTimer(withTimeInterval: 0.01, repeats: true) { timer in
                            self.penta1000.advanceFrame()
                            self.displayText = self.penta1000.display()

                            if !self.penta1000.isAnimating() {
                                timer.invalidate()
                            }
                        }
                    }
                }
            }
            .onEnded { _ in
                isTapped = false
            }
    }

    var body: some View {

        Image("penta1000")
            .onAppear(perform: appeared)
            .gesture(dragGesture)
            .accessibility(identifier: "calculator")
            .accessibility(label: Text(displayText))
            .overlay(DisplayView($displayText), alignment: .topTrailing)
            .padding(2000)
            .background(Color(red: 16/255, green: 16/255, blue: 16/255))
    }
}

struct Penta1000View_Previews: PreviewProvider {
    static var previews: some View {
        Penta1000View(penta1000: Penta1000(randomSeed: 0))
    }
}
