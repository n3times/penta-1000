import SwiftUI

// The display, composed of 12 14-segments LEDs. In addition, each LED has a dot to the
// right (decimal point).
struct DisplayView: View {
    // The text composed of up to 12 non-dot characters, each one optionally followed by a
    // a dot. The text should be right-justified within the display.
    @Binding var displayText: String

    private static let ledColor = Color.red
    private static let ledCount = 12
    private static let interLedX = 28.0
    private static let segmentCount = 14

    private static let standardWidth = 375

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
        0: CGRect(x: 0.0, y: 0, width: 3, height: 29),
        1: CGRect(x: 8.0, y: 0, width: 3, height: 29),
        2: CGRect(x: 16.0, y: 0, width: 3, height: 29),
        3: CGRect(x: 0.0, y: 13, width: 19, height: 3),
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
        0: [CGPoint(x: 2, y: 2), CGPoint(x: 2+2, y: 2), CGPoint(x: 17, y: 27-3),
            CGPoint(x: 17, y: 27), CGPoint(x: 17-2, y: 27), CGPoint(x: 2, y: 2+3)],
        1: [CGPoint(x: 1, y: 27-3), CGPoint(x: 18-2, y: 2), CGPoint(x: 18, y: 2),
            CGPoint(x: 18, y: 2+3), CGPoint(x: 1+2, y: 27), CGPoint(x: 1, y: 27)],
    ]

    private let dotData = CGRect(x: 21, y: 24.5, width: 4, height: 4)

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

    private func getLedPath(c: Character, startX: Double, hasDot:Bool) -> Path? {
        let segments = app_support_get_led_segments(Int8(c.asciiValue!))
        if (segments == 0) { return nil }
        var isSegmentResolved = [Bool](repeating: false, count: 14)

        var path = Path()

        if isSegment(segments: segments, i: 1) && isSegment(segments: segments, i: 8) {
            let segmentPath = getRectSegmentPath(rect: DisplayView.combinedRightSegmentsData[0]!)
            path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
            isSegmentResolved[1] = true
            isSegmentResolved[8] = true
        }
        if isSegment(segments: segments, i: 3) && isSegment(segments: segments, i: 10) {
            let segmentPath = getRectSegmentPath(rect: DisplayView.combinedRightSegmentsData[1]!)
            path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
            isSegmentResolved[3] = true
            isSegmentResolved[10] = true
        }
        if isSegment(segments: segments, i: 5) && isSegment(segments: segments, i: 12) {
            let segmentPath = getRectSegmentPath(rect: DisplayView.combinedRightSegmentsData[2]!)
            path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
            isSegmentResolved[5] = true
            isSegmentResolved[12] = true
        }
        if isSegment(segments: segments, i: 6) && isSegment(segments: segments, i: 7) {
            let segmentPath = getRectSegmentPath(rect: DisplayView.combinedRightSegmentsData[3]!)
            path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
            isSegmentResolved[6] = true
            isSegmentResolved[7] = true
        }
        if isSegment(segments: segments, i: 2) && isSegment(segments: segments, i: 11) {
            let segmentPath =
                getAngledSegmentPath(points: DisplayView.combinedAngledSegmentsData[0]!)
            path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
            isSegmentResolved[2] = true
            isSegmentResolved[11] = true
        }
        if isSegment(segments: segments, i: 4) && isSegment(segments: segments, i: 9) {
            let segmentPath =
                getAngledSegmentPath(points: DisplayView.combinedAngledSegmentsData[1]!)
            path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
            isSegmentResolved[4] = true
            isSegmentResolved[9] = true
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
            if (hasDot) {
                path.addRect(dotData.offsetBy(dx: CGFloat(startX), dy: CGFloat(0)))
            }
        }
        return path
    }

    init(_ displayText: Binding<String>) {
        self._displayText = displayText
    }

    func getPath(_ string: String) -> TransformedShape<Path> {
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
                                     hasDot: hasDot)
            if ledPath != nil { path.addPath(ledPath!) }
            index += 1
        }
        // Slant display slightly.
        let transformedPath =
            path.transform(CGAffineTransform.init(a: 1, b: 0, c: -0.08, d: 1, tx: 0, ty: 0))
        return transformedPath
    }

    func getView(_ metrics: GeometryProxy) -> some View {
        let scaleFactor = metrics.size.width / CGFloat(DisplayView.standardWidth)
        let fullRect = getPath("PENTATRONICS").shape.boundingRect
        return getPath(displayText)
            .scale(scaleFactor)
            .offset(x: (metrics.size.width - fullRect.width) / 2 * scaleFactor + 2.0,
                    y: (metrics.size.height - fullRect.height) / 2 * scaleFactor)
            .fill(DisplayView.ledColor)
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
