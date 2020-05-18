import XCTest
@testable import Penta_1000

class penta1000Tests: XCTestCase {

    override func setUpWithError() throws {
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }

    override func tearDownWithError() throws {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
    }

    func testEngine() throws {
        // Core.
        let p1 = Penta1000(randomSeed: 0)
        XCTAssertEqual(p1.display(), "PENTATRONICS")

        for c in "1+1=" {
            p1.pressKey(c: c)
        }
        XCTAssertEqual(p1.display(), "2")

        for c in "/0=" {
            p1.pressKey(c: c)
        }
        XCTAssertEqual(p1.display(), "DIV BY ZERO")

        p1.pressKey(c: "g")
        XCTAssertEqual(p1.display(), "> HI-LO GAME")

        // Animation.
        while p1.isAnimating() {
            p1.advanceFrame()
        }
        XCTAssertEqual(p1.display(), "___ GUESS ")

        // Logging.
        XCTAssertEqual(p1.firstAvailableLogEntryIndex(), 1)
        XCTAssertEqual(p1.lastAvailableLogEntryIndex(), 2)
        XCTAssertEqual(p1.logEntry(atIndex: 1), "1+1=2")
        XCTAssertEqual(p1.logEntry(atIndex: 2), "2/0=DIV BY ZERO")

        p1.clearLog()
        XCTAssertEqual(p1.firstAvailableLogEntryIndex(), 0)
        XCTAssertEqual(p1.lastAvailableLogEntryIndex(), 0)

        // Save Penta1000 to file system.
        let saved = p1.save(filename: "penta1000.dat")
        if (!saved) {
            XCTAssertNotNil(nil, "Couldn't save")
        }

        // Read Penta1000 from file system.
        let p1FromFile = Penta1000(filename: "penta1000.dat")
        if (p1FromFile != nil) {
            XCTAssertEqual(p1FromFile!.display(), "___ GUESS ")
        } else {
            XCTAssertNotNil(nil, "Couldn't read file")
        }
    }

    func testPerformanceExample() throws {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }

}
