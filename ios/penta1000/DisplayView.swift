import SwiftUI

// The display, composed of 12 14-segments LEDs. In addition, each LED has a dot to the
// right (decimal point).
struct DisplayView: View {
    // The text composed of up to 12 non-dot characters, each one optionally followed by a
    // a dot. The text should be right-justified within the display.
    @Binding var displayText: String

    private static let ledColor = Color(red: 255/255, green: 0/255, blue: 0/255)
    private static let ledCount = 12
    private static let interLedX = 28.0
    private static let segmentCount = 14

    private static let standardizedWidth = 375.0
    private static let ledWidth = 20.0
    private static let allLedsWidth =
        Double(DisplayView.ledCount - 1) * DisplayView.interLedX + DisplayView.ledWidth
    private static let x0 =
        (DisplayView.standardizedWidth - DisplayView.allLedsWidth) / 2
    // display height: 96, font height: 29
    private static let y0 = (96.0 - 29.0) / 2

    // Describes the 10 horizontal and vertical segments.
    private let rightSegmentsData = [
        0: CGRect(x: 0.0, y: 0, width: 20, height: 3),
        1: CGRect(x: 0.0, y: 0, width: 3, height: 15),
        3: CGRect(x: 8.0, y: 0, width: 3, height: 15),
        5: CGRect(x: 16.0, y: 0, width: 3, height: 15),
        6: CGRect(x: 0.0, y: 13, width: 10, height: 3),
        7: CGRect(x: 9.0, y: 13, width: 11, height: 3),
        8: CGRect(x: 0.0, y: 14, width: 3, height: 15),
        10: CGRect(x: 8.0, y: 14, width: 3, height: 15),
        12: CGRect(x: 16.0, y: 14, width: 3, height: 15),
        13: CGRect(x: 0.0, y: 26, width: 20, height: 3),
    ]

    // Describes the 4 angled segments.
    private let angledSegmentsData = [
        2: [CGPoint(x: 1, y: 1), CGPoint(x: 1+2, y: 1), CGPoint(x: 10, y: 16-3),
            CGPoint(x: 10, y: 16), CGPoint(x: 10-2, y: 16), CGPoint(x: 1, y: 1+3)],
        4: [CGPoint(x: 19, y: 1), CGPoint(x: 19-2, y: 1), CGPoint(x: 9, y: 15-3),
            CGPoint(x: 9, y: 15), CGPoint(x: 9+2, y: 15), CGPoint(x: 19, y: 1+3)],
        9: [CGPoint(x: 1, y: 27), CGPoint(x: 1+2, y: 27), CGPoint(x: 9, y: 15+3),
            CGPoint(x: 9, y: 15), CGPoint(x: 9-2, y: 15), CGPoint(x: 1, y: 27-3)],
        11: [CGPoint(x: 19, y: 27), CGPoint(x: 19-2, y: 27), CGPoint(x: 10, y: 16+3),
            CGPoint(x: 10, y: 16), CGPoint(x: 10+2, y: 16), CGPoint(x: 19, y: 27-3)],
    ]

    private let dotData = CGRect(x: 21, y: 25, width: 4, height: 4)

    private func getAngledSegmentPath(index: Int) -> Path? {
        let points = angledSegmentsData[index]!
        var path = Path()
        path.move(to: points[0])
        for i in 1...5 {
            path.addLine(to: points[i])
        }
        return path
    }

    private func getRightSegmentPath(index: Int) -> Path? {
        let rect = rightSegmentsData[index]!
        var path = Path()
        path.move(to: CGPoint(x: rect.minX, y: rect.minY + 2))
        path.addLine(to: CGPoint(x: rect.minX + 2, y: rect.minY))
        path.addLine(to: CGPoint(x: rect.maxX - 2, y: rect.minY))
        path.addLine(to: CGPoint(x: rect.maxX, y: rect.minY + 2))
        path.addLine(to: CGPoint(x: rect.maxX, y: rect.maxY - 2))
        path.addLine(to: CGPoint(x: rect.maxX - 2, y: rect.maxY))
        path.addLine(to: CGPoint(x: rect.minX + 2, y: rect.maxY))
        path.addLine(to: CGPoint(x: rect.minX, y: rect.maxY - 2))
        return path
    }

    private func getLedPath(c: Character, startX: Double, startY: Double, hasDot:Bool) -> Path? {
        let segments = app_support_get_led_segments(Int8(c.asciiValue!))
        if (segments == 0) { return nil }

        var path = Path()
        for i in 0...(DisplayView.segmentCount - 1) {
            let segmentBit = segments & (1 << (DisplayView.segmentCount - 1 - i))
            if (segmentBit != 0) {
                let segmentPath: Path?
                let isAngled = i == 2 || i == 4 || i == 9 || i == 11
                if isAngled {
                    segmentPath = getAngledSegmentPath(index: i)
                } else {
                    segmentPath = getRightSegmentPath(index: i)
                }
                if (segmentPath != nil) {
                    path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(startY)))
                }
            }
            if (hasDot) {
                path.addEllipse(in: dotData.offsetBy(dx: CGFloat(startX), dy: CGFloat(startY)))
            }
        }
        return path
    }

    init(_ displayText: Binding<String>) {
        self._displayText = displayText
    }

    var body: some View {
        var path = Path()
        let displayCharacters = Array(displayText)
        var nonDotCount = 0
        for c in displayCharacters {
            if c != "." {
                nonDotCount += 1
            }
        }
        var index = 0
        for i in 0..<displayCharacters.count {
            if displayCharacters[i] == "." { continue }

            // Right justify.
            let position = index + (DisplayView.ledCount - nonDotCount)
            let hasDot = i < displayCharacters.count - 1 && displayCharacters[i + 1] == "."
            let ledPath = getLedPath(c: displayCharacters[i],
                                     startX: DisplayView.x0 + DisplayView.interLedX * Double(position),
                                     startY: DisplayView.y0,
                                     hasDot: hasDot)
            if ledPath != nil { path.addPath(ledPath!) }
            index += 1
        }
        return path
            .transform(CGAffineTransform.init(a: 1, b: 0, c: -0.08, d: 1, tx: 0, ty: 0))
            .offset(x: 5.0, y: 0) // tweak because of slanted font
            .fill(DisplayView.ledColor)
    }
}

struct DisplayView_Previews: PreviewProvider {
    static var previews: some View {
        DisplayView(.constant("READY"))
    }
}
