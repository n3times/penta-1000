import SwiftUI

struct DisplayView: View {
    @Binding var displayText: String

    let ledColor = Color(red: 255/255, green: 64/255, blue: 64/255)

    let charToSegments10 = [
        "0": 0b1101001011,
        "1": 0b0001000010,
        "2": 0b1001111001,
        "3": 0b1001110011,
        "4": 0b0101110010,
        "5": 0b1100110011,
        "6": 0b1100111011,
        "7": 0b1001000010,
        "8": 0b1101111011,
        "9": 0b1101110011,
        "A": 0b1101111010,
        "B": 0b0100111011,
        "C": 0b1100001001,
        "D": 0b0001111011,
        "E": 0b1100111001,
        "F": 0b1100111000,
        "G": 0b1100001011,
        "H": 0b0101111010,
        "I": 0b0001000010,
        "J": 0b0001000011,
        "L": 0b0100001001,
        "N": 0b0000111010,
        "O": 0b1101001011,
        "P": 0b1101111000,
        "R": 0b0000111000,
        "S": 0b1100110011,
        "T": 0b0100111000,
        "U": 0b0101001011,
        "V": 0b0101001011,
        "W": 0b1000001011,
        "Y": 0b0101110010,
        "Z": 0b1001111001,
        "_": 0b0000000001,
        "-": 0b0000110000,
        "/": 0b0000000011,
        "*": 0b0101111010,
        "+": 0b0010110100,
        "%": 0b0001111000,
    ]

    let segmentRects10 = [
        0: CGRect(x: 2.0, y: 0, width: 16, height: 4),
        1: CGRect(x: 0.0, y: 2, width: 4, height: 16),
        2: CGRect(x: 8.0, y: 4, width: 4, height: 14),
        3: CGRect(x: 16.0, y: 2, width: 4, height: 16),
        4: CGRect(x: 2.0, y: 16, width: 8, height: 4),
        5: CGRect(x: 10.0, y: 16, width: 8, height: 4),
        6: CGRect(x: 0.0, y: 18, width: 4, height: 16),
        7: CGRect(x: 8.0, y: 18, width: 4, height: 14),
        8: CGRect(x: 16.0, y: 18, width: 4, height: 16),
        9: CGRect(x: 2.0, y: 32, width: 16, height: 4),
    ]

    let dotData = CGRect(x: 22, y: 31, width: 5, height: 5)

    init(_ displayText: Binding<String>) {
        self._displayText = displayText
    }

    func getSegmentPath(index: Int) -> Path {
        let rect = segmentRects10[index]!
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

    func getLedPath(c: Character, startX: Double, startY: Double, hasDot:Bool) -> Path? {
        let data = charToSegments10[String(c)]
        if (data == nil) { return nil }

        var path = Path()
        for i in 0...9 {
            let isSegmentOn = data! & (1 << (9 - i)) != 0
            if (isSegmentOn) {
                let segmentPath = getSegmentPath(index: i)
                path.addPath(segmentPath.offsetBy(dx: CGFloat(startX), dy: CGFloat(startY)))
            }
            if (hasDot) {
                path.addEllipse(
                    in: dotData.offsetBy(dx: CGFloat(startX), dy: CGFloat(startY)))
            }
        }
        return path
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
                let startY = 225.0
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
