import SwiftUI

/// The main view. It holds the calculator with its keyboard and display.
struct Penta1000View: View {
    private let penta1000: Penta1000
    @State private var displayText = "READY"

    init() {
        self.penta1000 = Penta1000.shared
    }

    var body: some View {
        return GeometryReader { geometry in
            let standardCalcWidth = 375.0
            let standardCalcHeight = 647.0
            let standardDisplayHeight = 96.0
            let standardDisplayOffsetY = 195.0

            // Maximize the size of the calculator while maintaining the aspect ratio.

            let screenWidth = Double(geometry.size.width)
            let screenHeight = Double(geometry.size.height)

            let screenAspectRatio = screenHeight / screenWidth
            let calcAspectRatio = standardCalcHeight / standardCalcWidth
            let isPortrait = screenAspectRatio >= calcAspectRatio

            let calcWidth = isPortrait ? screenWidth : screenHeight / calcAspectRatio
            let calcHeight = isPortrait ? screenWidth * calcAspectRatio : screenHeight

            // Compute the size and the location of the display.

            let scaleFactor = calcWidth / standardCalcWidth

            let displayWidth = calcWidth
            let displayHeight = standardDisplayHeight * scaleFactor

            let displayOffsetX = CGFloat(0.0)
            let displayOffsetY =
                (standardDisplayOffsetY - (standardCalcHeight - standardDisplayHeight)/2) * scaleFactor

            ZStack {
                Color(red: 16.0/255, green: 16.0/255, blue: 16.0/255).edgesIgnoringSafeArea(.all)
                KeyboardView(penta1000: penta1000, displayText: $displayText)
                    .frame(width: CGFloat(calcWidth), height: CGFloat(calcHeight), alignment: .center)
                DisplayView(displayString: $displayText)
                    .offset(x: CGFloat(displayOffsetX), y: CGFloat(displayOffsetY))
                    .frame(width: CGFloat(displayWidth),
                           height: CGFloat(displayHeight),
                           alignment: .center)
                    .onAppear {
                        displayText = penta1000.display()
                    }
            }
        }
    }
}

struct Penta1000View_Previews: PreviewProvider {
    static var previews: some View {
        Penta1000View()
    }
}
