import SwiftUI

struct DisplayView: View {
    @Binding var displayText: String

    let ledColor = Color(red: 255/255, green: 64/255, blue: 64/255)

    let charToSegments = [
        "0": 0b1110111,
        "1": 0b0010010,
        "2": 0b1011101,
        "3": 0b1011011,
        "4": 0b0111010,
        "5": 0b1101011,
        "6": 0b1101111,
        "7": 0b1010010,
        "8": 0b1111111,
        "9": 0b1111011,
        "A": 0b1111110,
        "C": 0b1100101,
        "D": 0b0011111,
        "E": 0b1101101,
        "F": 0b1101100,
        "G": 0b1100111,
        "H": 0b0111110,
        "I": 0b0010010,
        "L": 0b0100101,
        "N": 0b0001110,
        "O": 0b1110111,
        "P": 0b1111100,
        "R": 0b0001100,
        "S": 0b1101011,
        "T": 0b0101100,
        "U": 0b0110111,
        "W": 0b1000111,
        "Y": 0b0111010,
        "_": 0b0000001,
        "-": 0b0001000,
        "/": 0b0000011,
        "*": 0b0111110,
        "+": 0b0011010,
    ]

    let segmentsRects = [
        0: CGRect(x: 0.0, y: 0, width: 20, height: 4),
        1: CGRect(x: 0.0, y: 0, width: 4, height: 18),
        2: CGRect(x: 16.0, y: 0, width: 4, height: 18),
        3: CGRect(x: 0.0, y: 16, width: 20, height: 4),
        4: CGRect(x: 0.0, y: 18, width: 4, height: 18),
        5: CGRect(x: 16.0, y: 18, width: 4, height: 18),
        6: CGRect(x: 0.0, y: 32, width: 20, height: 4),
    ]

    let dotData = CGRect(x: 22, y: 31, width: 5, height: 5)

    init(_ displayText: Binding<String>) {
        self._displayText = displayText
    }

    func getLedPath(c: Character, startX: Double, startY: Double) -> Path? {
        let data = charToSegments[String(c)]
        if (data == nil) { return nil }

        var path = Path()
        for i in 0...6 {
            let isSegmentOn = data! & (1 << (6 - i)) != 0
            if (isSegmentOn) {
                let rectPath =
                    Path(segmentsRects[i]!).offsetBy(dx: CGFloat(startX), dy: CGFloat(startY))
                path.addPath(rectPath)
            }
            /// TODO: add dot when appropriate.
            ///path.addEllipse(in: dotData.offsetBy(dx: CGFloat(startX), dy: CGFloat(startY)))
        }
        return path
    }

    var body: some View {
        let leds = Array(displayText)

        return Path { path in
            for i in 0...11 {
                let startX = 9 + 30 * Double(i)
                let startY = 225.0
                let led = leds[i]
                let ledPath = getLedPath(c: led, startX: startX, startY: startY)
                if ledPath != nil { path.addPath(ledPath!) }
            }

        }.transform(CGAffineTransform.init(a: 1, b: 0, c: -0.08, d: 1, tx: 0, ty: 0))
         .fill(ledColor)
    }
}

struct DisplayView_Previews: PreviewProvider {
    static var previews: some View {
        DisplayView(.constant("       READY"))
    }
}
