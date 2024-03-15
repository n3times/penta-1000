import XCTest

class penta1000UITests: XCTestCase {

    override func setUpWithError() throws {
        // Put setup code here. This method is called before the invocation of each test method in the class.

        // In UI tests it is usually best to stop immediately when a failure occurs.
        continueAfterFailure = false

        // In UI tests it’s important to set the initial state - such as interface orientation - required for your tests before they run. The setUp method is a good place to do this.
    }

    override func tearDownWithError() throws {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
    }

    func testFunctionality() throws {
        let app = XCUIApplication()
        app.launchArguments.append("--reset")
        app.launch()
        let calculator = app.images["calculator"]

        XCTAssertEqual(calculator.label, "PENTATRONICS")

        tap(app: app, character: "1")
        XCTAssertEqual(calculator.label, "1")
        tap(app: app, character: "+")
        XCTAssertEqual(calculator.label, "1+")
        tap(app: app, character: "1")
        XCTAssertEqual(calculator.label, "1+1")
        tap(app: app, character: "=")
        XCTAssertEqual(calculator.label, "2")
        tap(app: app, character: "c")
        XCTAssertEqual(calculator.label, "READY")
    }

    func testLaunchPerformance() throws {
        measure(metrics: [XCTApplicationLaunchMetric()]) {
            XCUIApplication().launch()
        }
    }

    private func tap(app: XCUIApplication, character: Character) {
        let calculator = app.images["calculator"]
        let calculatorOrigin =
            calculator.coordinate(withNormalizedOffset: CGVector(dx: 0, dy: 0))
        let characterLocation = location(forCharacter: character)
        let factor = calculator.frame.width / 375
        let characterCoordinate = calculatorOrigin.withOffset(CGVector(
            dx: characterLocation!.x * factor, dy: characterLocation!.y * factor))
        characterCoordinate.tap()
    }

    private func location(forCharacter: Character) -> CGPoint? {
        let keys = ["g~%c", "789/", "456*", "123+", "0.-="]

        // Top left corner of top left key ("?").
        let x0: CGFloat = 28.0
        let y0: CGFloat = 309.0

        // Separation between the top left corners of consecutive keys.
        let dx: CGFloat = 84.5
        let dy: CGFloat = 67.0

        for row in 0...4 {
            let chars = Array(keys[row])
            for col in 0...3 {
                if chars[col] == forCharacter {
                    return CGPoint(x: x0 + CGFloat(col) * dx, y: y0 + CGFloat(row) * dy)
                }
            }
        }
        return nil
    }
}
