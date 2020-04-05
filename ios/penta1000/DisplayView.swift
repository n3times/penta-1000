import SwiftUI

// The display, composed of 12 14-segments LEDs. In addition, each LED has a dot to the
// right (decimal point).
struct DisplayView: View {
    // The text composed of up to 12 non-dot characters, each one optionally followed by a
    // a dot. The text should be right-justified within the display.
    @Binding var displayText: String

    private let ledColor = Color(red: 240/255, green: 240/255, blue: 240/255)
    private let ledCount = 12
    private let x0 = 43.0
    private let y0 = 231.0
    private let interLedX = 28.0
    private let segmentCount = 14

    // Indicates, for each character, which segments are on.
    private let charToSegments = [
        // Segments top to bottom, left to right.
        //     -|\|/|--|/|\|-
        "0": 0b11000100100011,
        "1": 0b00010000001000,
        "2": 0b10000111100001,
        "3": 0b10000111000011,
        "4": 0b01000111000010,
        "5": 0b11000011000011,
        "6": 0b11000011100011,
        "7": 0b10000100000010,
        "8": 0b11000111100011,
        "9": 0b11000111000011,
        "A": 0b11000111100010,
        "B": 0b10010101001011,
        "C": 0b11000000100001,
        "D": 0b10010100001011,
        "E": 0b11000011100001,
        "e": 0b00000010110001,
        "F": 0b11000011100000,
        "G": 0b11000001100011,
        "H": 0b01000111100010,
        "I": 0b10010000001001,
        "J": 0b00000100000011,
        "K": 0b01001010100100,
        "L": 0b01000000100001,
        "M": 0b01101100100010,
        "N": 0b01100100100110,
        "O": 0b11000100100011,
        "P": 0b11000111100000,
        "Q": 0b11000100100111,
        "R": 0b11000111100100,
        "S": 0b11000011000011,
        "T": 0b10010000001000,
        "U": 0b01000100100011,
        "V": 0b01001000110000,
        "W": 0b01000100110110,
        "X": 0b00101000010100,
        "Y": 0b00101000001000,
        "Z": 0b10001000010001,
        "_": 0b00000000000001,
        "-": 0b00000011000000,
        "/": 0b00001000010000,
        "*": 0b00111011011100,
        "+": 0b00010011001000,
        "%": 0b01101011010110,
        "?": 0b10000101001000,
        ">": 0b00100000010000,
    ]

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
        let segments = charToSegments[String(c)]
        if (segments == nil) { return nil }

        var path = Path()
        for i in 0...(segmentCount - 1) {
            let segmentBit = segments! & (1 << (segmentCount - 1 - i))
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
            let position = index + (ledCount - nonDotCount)
            let hasDot = i < displayCharacters.count - 1 && displayCharacters[i + 1] == "."
            let ledPath = getLedPath(c: displayCharacters[i],
                                     startX: x0 + interLedX * Double(position),
                                     startY: y0,
                                     hasDot: hasDot)
            if ledPath != nil { path.addPath(ledPath!) }
            index += 1
        }
        return path.transform(CGAffineTransform.init(a: 1, b: 0, c: -0.08, d: 1, tx: 0, ty: 0))
                   .fill(ledColor)
    }
}

struct DisplayView_Previews: PreviewProvider {
    static var previews: some View {
        DisplayView(.constant("READY"))
    }
}
