import SwiftUI
import AudioToolbox

/// The main view. It holds the calculator with its keyboard and display.
struct Penta1000View: View {
    private let penta1000: Penta1000
    @State private var displayText = "READY"

    init() {
        self.penta1000 = Penta1000.shared
    }

    var body: some View {
        return GeometryReader { metrics in
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
                KeyboardView(penta1000: penta1000, displayText: $displayText)
                    .frame(width: CGFloat(calcWidth), height: CGFloat(calcHeight), alignment: .center)
                DisplayView(self.$displayText)
                    .offset(x: CGFloat(displayOffsetX), y: CGFloat(displayOffsetY))
                    .frame(width: CGFloat(displayWidth),
                           height: CGFloat(displayHeight),
                           alignment: .center)
                    .onAppear {
                        self.displayText = self.penta1000.display()
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
