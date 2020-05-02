import SwiftUI

// The display, composed of 12 14-segments LEDs. In addition, each LED has a dot to the
// right (decimal point).
struct DisplayView: View {
    // The text composed of up to 12 non-dot characters, each one optionally followed by a
    // a dot. The text should be right-justified within the display.
    @Binding var displayText: String

    private static let ledColor = Color.red
    private static let ledCount = 12
    private static let ledHeight = CGFloat(29.0)
    private static let interLedX = 30.0
    private static let segmentCount = 14
    private static let slant = CGFloat(0.08)
    private static let displayToCalculatorWidthRatio = CGFloat(0.85)

    // Describes the 10 horizontal and vertical segments.
    private static let rightSegmentsData = [
        0: CGRect(x: 0.0, y: 0, width: 19, height: 3),
        1: CGRect(x: 0.0, y: 0, width: 3, height: 15),
        3: CGRect(x: 8.0, y: 0, width: 3, height: 15),
        5: CGRect(x: 16.0, y: 0, width: 3, height: 15),
        6: CGRect(x: 0.0, y: 13, width: 10, height: 3),
        7: CGRect(x: 9.0, y: 13, width: 10, height: 3),
        8: CGRect(x: 0.0, y: 14, width: 3, height: 15),
        10: CGRect(x: 8.0, y: 14, width: 3, height: 15),
        12: CGRect(x: 16.0, y: 14, width: 3, height: 15),
        13: CGRect(x: 0.0, y: 26, width: 19, height: 3),
    ]

    private static let combinedRightSegmentsData = [
        [1, 8]: CGRect(x: 0.0, y: 0, width: 3, height: 29),
        [3, 10]: CGRect(x: 8.0, y: 0, width: 3, height: 29),
        [5, 12]: CGRect(x: 16.0, y: 0, width: 3, height: 29),
        [6, 7]: CGRect(x: 0.0, y: 13, width: 19, height: 3),
    ]

    // Describes the 4 angled segments. Points of each hexagon should be listed clockwise.
    private static let angledSegmentsData = [
        2: [CGPoint(x: 2, y: 2), CGPoint(x: 2+2, y: 2), CGPoint(x: 10, y: 15-3),
            CGPoint(x: 10, y: 15), CGPoint(x: 10-2, y: 15), CGPoint(x: 2, y: 2+3)],
        4: [CGPoint(x: 18, y: 2+3), CGPoint(x: 9+2, y: 15), CGPoint(x: 9, y: 15),
            CGPoint(x: 9, y: 15-3), CGPoint(x: 18-2, y: 2), CGPoint(x: 18, y: 2)],
        9: [CGPoint(x: 1, y: 27-3), CGPoint(x: 10-2, y: 15), CGPoint(x: 10, y: 15),
            CGPoint(x: 10, y: 15+3), CGPoint(x: 1+2, y: 27), CGPoint(x: 1, y: 27)],
        11: [CGPoint(x: 18, y: 27), CGPoint(x: 18-2, y: 27), CGPoint(x: 9, y: 14+3),
            CGPoint(x: 9, y: 14), CGPoint(x: 9+2, y: 14), CGPoint(x: 18, y: 27-3)],
    ]

    private static let combinedAngledSegmentsData = [
        [2, 11]: [CGPoint(x: 2, y: 2), CGPoint(x: 2+2, y: 2), CGPoint(x: 17, y: 27-3),
                  CGPoint(x: 17, y: 27), CGPoint(x: 17-2, y: 27), CGPoint(x: 2, y: 2+3)],
        [4, 9]: [CGPoint(x: 1, y: 27-3), CGPoint(x: 18-2, y: 2), CGPoint(x: 18, y: 2),
                 CGPoint(x: 18, y: 2+3), CGPoint(x: 1+2, y: 27), CGPoint(x: 1, y: 27)],
    ]

    private let dotData = CGRect(x: 21.5, y: 25, width: 4, height: 4)

    private func getRectSegmentPath(rect: CGRect) -> Path? {
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

    private func getAngledSegmentPath(points: [CGPoint]) -> Path? {
        var path = Path()
        path.move(to: points[0])
        for i in 1...5 {
            path.addLine(to: points[i])
        }
        return path
    }

    private func isSegment(segments: Int32, i: Int) -> Bool {
        return segments & (1 << (DisplayView.segmentCount - 1 - i)) != 0
    }

    private func getLedPath(c: Character, startX: Double, hasDot: Bool, combineSegments:Bool)
            -> Path? {
        let segments = app_support_get_led_segments(Int8(c.asciiValue!))
        if (segments == 0) { return nil }
        var isSegmentResolved = [Bool](repeating: false, count: 14)

        var path = Path()

        if (combineSegments) {
            for pair in DisplayView.combinedRightSegmentsData.keys {
                if isSegment(segments: segments, i: pair[0]) &&
                   isSegment(segments: segments, i: pair[1]) {
                    let segmentPath =
                        getRectSegmentPath(rect: DisplayView.combinedRightSegmentsData[pair]!)
                    path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
                    isSegmentResolved[pair[0]] = true
                    isSegmentResolved[pair[1]] = true
                }
            }
            for pair in DisplayView.combinedAngledSegmentsData.keys {
                if isSegment(segments: segments, i: pair[0]) &&
                   isSegment(segments: segments, i: pair[1]) {
                    let segmentPath =
                        getAngledSegmentPath(points: DisplayView.combinedAngledSegmentsData[pair]!)
                    path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
                    isSegmentResolved[pair[0]] = true
                    isSegmentResolved[pair[1]] = true
                }
            }
        }

        for i in 0...(DisplayView.segmentCount - 1) {
            if isSegment(segments: segments, i: i) {
                let segmentPath: Path?
                let isAngled = i == 2 || i == 4 || i == 9 || i == 11
                if isAngled {
                    segmentPath =
                        getAngledSegmentPath(points: DisplayView.angledSegmentsData[i]!)
                } else {
                    segmentPath = getRectSegmentPath(rect: DisplayView.rightSegmentsData[i]!)
                }
                if (segmentPath != nil && !isSegmentResolved[i]) {
                    path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
                }
            }
        }
        if (hasDot) {
            path.addRect(dotData.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
        }
        return path
    }

    private func getPath(_ string: String) -> TransformedShape<Path> {
        var path = Path()
        let displayCharacters = Array(string)
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
                                     startX: DisplayView.interLedX * Double(position),
                                     hasDot: hasDot,
                                     combineSegments: true)
            if ledPath != nil { path.addPath(ledPath!) }
            index += 1
        }
        // Slant display slightly
        let slantedPath = path.transform(CGAffineTransform.init(a: 1,
                                                                b: 0,
                                                                c: -DisplayView.slant,
                                                                d: 1,
                                                                tx: 0,
                                                                ty: 0))
        return slantedPath
    }

    private func getView(_ metrics: GeometryProxy) -> some View {
        let fullRect = getPath("PENTATRONICS").shape.boundingRect
        let scaleFactor: CGFloat
            = DisplayView.displayToCalculatorWidthRatio * metrics.size.width / fullRect.width
        var recenterOffsetX = DisplayView.ledHeight * DisplayView.slant
        // Tweak so that it *looks* more centered (even if it is not).
        recenterOffsetX += 1

        let offsetX = (metrics.size.width - fullRect.width + recenterOffsetX) / 2 * scaleFactor
        let offsetY = (metrics.size.height - fullRect.height) / 2 * scaleFactor

        return getPath(displayText)
            .scale(scaleFactor)
            .offset(x: offsetX, y: offsetY)
            .fill(DisplayView.ledColor)
    }

    init(_ displayText: Binding<String>) {
        self._displayText = displayText
    }

    var body: some View {
      GeometryReader { metrics in
        self.getView(metrics)
      }
    }
}

struct DisplayView_Previews: PreviewProvider {
    static var previews: some View {
        DisplayView(.constant("READY"))
    }
}
