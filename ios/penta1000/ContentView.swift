import SwiftUI

struct ContentView: View {
    let p1 = Penta1000(randomSeed: 0)

    // Returns the key at a given location, or nil if there is no such a key.
    func getCalculatorKey(location: CGPoint) -> Character? {
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

        let i: CGFloat = (x - x0) / dx
        let j: CGFloat = (y - y0) / dy

        if (i >= 0 && j >= 0 && i < 4 && j < 5) {
            if i - CGFloat(Int(i)) > w / dx { return nil }
            if j - CGFloat(Int(j)) > h / dy { return nil }
            let keys = ["g~%c", "789/", "456*", "123+", "0.-="]
            let index = keys[Int(j)].index(keys[Int(j)].startIndex, offsetBy: Int(i))
            return keys[Int(j)][index]
        } else {
            return nil
        }
    }

    // Listens for a tap event to determine if user pressed a calculator key.
    private var dragGesture: some Gesture {
        DragGesture(minimumDistance: 0, coordinateSpace: .local)
            .onEnded {
                let c = self.getCalculatorKey(location: $0.location)
                if c != nil {
                    self.p1.pressKey(c: c!)
                    // Skip animation.
                    for _ in 0...500 {
                        if self.p1.isAnimating() {
                            self.p1.advanceFrame()
                        } else {
                            break
                        }
                    }
                    print(self.p1.display())
                }
        }
    }

    var body: some View {
        Image("penta1000")
            .background(Color.black)
            .gesture(dragGesture)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
