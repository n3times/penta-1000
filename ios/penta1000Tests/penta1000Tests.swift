//
//  penta1000Tests.swift
//  penta1000Tests
//
//  Created by Paul Novaes on 3/27/20.
//  Copyright © 2020 Paul Novaes. All rights reserved.
//

import XCTest
@testable import penta1000

class penta1000Tests: XCTestCase {

    override func setUpWithError() throws {
        // Put setup code here. This method is called before the invocation of each test method in the class.
    }

    override func tearDownWithError() throws {
        // Put teardown code here. This method is called after the invocation of each test method in the class.
    }

    func testEngine() throws {
        // Core.
        let p1 = Penta1000(seed: 0)
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
        XCTAssertEqual(p1.display(), "___         ")

        // Logging.
        XCTAssertEqual(p1.firstAvailableLogEntryIndex(), 1)
        XCTAssertEqual(p1.lastAvailableLogEntryIndex(), 2)
        XCTAssertEqual(p1.logEntry(atIndex: 1), "1+1=2")
        XCTAssertEqual(p1.logEntry(atIndex: 2), "2/0=DIV BY ZERO")

        p1.clearLog()
        XCTAssertEqual(p1.firstAvailableLogEntryIndex(), 0)
        XCTAssertEqual(p1.lastAvailableLogEntryIndex(), 0)

        // Save State.
        let state = p1.state()
        XCTAssertNotNil(state)

        let p1State = p1.state()
        let p1Data = Data(bytes: p1State.state, count: p1State.size)
        let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
        let p1FileURL = dir?.appendingPathComponent("penta1000.dat")
        XCTAssertNotNil(p1FileURL)

        if (p1FileURL != nil) {
            do {
                try p1Data.write(to: p1FileURL!, options: .atomic)
            } catch {
                XCTAssertNotNil(nil, "Couldn't write file")
            }
        }
        ///engine_release_state(state: state);

        // Read State.
        if (p1FileURL != nil) {
            var fileData: Data? = nil
            do {
                try fileData = Data(contentsOf: p1FileURL!)
            } catch {
                XCTAssertNotNil(nil, "Couldn't read file")
            }
            if (fileData != nil) {
                var p1FromFile: Penta1000?
                fileData!.withUnsafeBytes {
                    p1FromFile = Penta1000(state: $0)}
                XCTAssertEqual(p1FromFile!.display(), "___         ")
                p1FromFile = nil
            }
        }
    }

    func testPerformanceExample() throws {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }

}
