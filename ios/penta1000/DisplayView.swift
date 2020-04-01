import SwiftUI

let displayColor = Color(red: 233/255, green: 233/255, blue: 233/255)

struct DisplayView: View {
    @Binding var displayText: String

    init(_ displayText: Binding<String>) {
        self._displayText = displayText
    }

    var body: some View {
        Text(displayText)
            .font(.system(size: 35, weight: .bold, design: .monospaced))
            .foregroundColor(displayColor)
            .position(x: 185, y: 245)
    }
}

struct DisplayView_Previews: PreviewProvider {
    static var previews: some View {
        DisplayView(.constant("       READY"))
    }
}
