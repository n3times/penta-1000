import SwiftUI

struct DisplayView: View {
    @Binding var displayText: String

    private let ledColor = Color(red: 240/255, green: 240/255, blue: 240/255)

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

    private let segmentRects = [
        0: CGRect(x: 2.0, y: 0, width: 16, height: 4),
        1: CGRect(x: 0.0, y: 2, width: 4, height: 14),
        3: CGRect(x: 8.0, y: 2, width: 4, height: 14),
        5: CGRect(x: 16.0, y: 2, width: 4, height: 14),
        6: CGRect(x: 2.0, y: 14, width: 8, height: 4),
        7: CGRect(x: 10.0, y: 14, width: 8, height: 4),
        8: CGRect(x: 0.0, y: 16, width: 4, height: 14),
        10: CGRect(x: 8.0, y: 16, width: 4, height: 14),
        12: CGRect(x: 16.0, y: 16, width: 4, height: 14),
        13: CGRect(x: 2.0, y: 28, width: 16, height: 4),
    ]

    private let angledSegments = [
        2: [CGPoint(x: 1, y: 1), CGPoint(x: 1+1, y: 1), CGPoint(x: 10, y: 16-4),
            CGPoint(x: 10, y: 16), CGPoint(x: 10-1, y: 16), CGPoint(x: 1, y: 1+4)],
        4: [CGPoint(x: 19, y: 1), CGPoint(x: 19-1, y: 1), CGPoint(x: 10, y: 16-4),
            CGPoint(x: 10, y: 16), CGPoint(x: 10+1, y: 16), CGPoint(x: 19, y: 1+4)],
        9: [CGPoint(x: 1, y: 31), CGPoint(x: 1+1, y: 31), CGPoint(x: 10, y: 16+4),
            CGPoint(x: 10, y: 16), CGPoint(x: 10-1, y: 16), CGPoint(x: 1, y: 31-4)],
        11: [CGPoint(x: 19, y: 31), CGPoint(x: 19-1, y: 31), CGPoint(x: 10, y: 16+4),
            CGPoint(x: 10, y: 16), CGPoint(x: 10+1, y: 16), CGPoint(x: 19, y: 31-4)],
    ]

    private let dotData = CGRect(x: 21, y: 27, width: 5, height: 5)

    private func getAngledPath(index: Int) -> Path {
        let points = angledSegments[index]!
        var path = Path()
        path.move(to: points[0])
        for i in 1...5 {
            path.addLine(to: points[i])
        }
        return path
    }

    private func getSegmentPath(index: Int) -> Path? {
        if (index == 2 || index == 4 || index == 9 || index == 11) {
            return getAngledPath(index: index)
        }

        let rect = segmentRects[index]!
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
        let data = charToSegments[String(c)]
        if (data == nil) { return nil }

        var path = Path()
        for i in 0...13 {
            let isSegmentOn = data! & (1 << (13 - i)) != 0
            if (isSegmentOn) {
                let segmentPath = getSegmentPath(index: i)
                if (segmentPath != nil) {
                    path.addPath(segmentPath!.offsetBy(dx: CGFloat(startX), dy: CGFloat(startY)))
                }
            }
            if (hasDot) {
                path.addEllipse(
                    in: dotData.offsetBy(dx: CGFloat(startX), dy: CGFloat(startY)))
            }
        }
        return path
    }

    init(_ displayText: Binding<String>) {
        self._displayText = displayText
    }

    var body: some View {
        Path { path in
            let characters = Array(displayText)
            var dotCount = 0
            for i in 0...15 {
                let c = characters[i]
                if c == "." { dotCount += 1 }
            }
            var index = 0
            for i in (4 - dotCount)...15 {
                let c = characters[i]
                if c == "." { continue }
                let hasDot = i < 15 && characters[i + 1] == "."
                let startX = 30 + 30 * Double(index)
                let startY = 229.0
                let ledPath =
                    getLedPath(c: c, startX: startX, startY: startY, hasDot: hasDot)
                if ledPath != nil { path.addPath(ledPath!) }
                index += 1
            }
        }.transform(CGAffineTransform.init(a: 1, b: 0, c: -0.08, d: 1, tx: 0, ty: 0))
         .fill(ledColor)
    }
}

struct DisplayView_Previews: PreviewProvider {
    static var previews: some View {
        DisplayView(.constant("           READY"))
    }
}
