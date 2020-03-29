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
        XCTAssertEqual(p1.display(), "___         ")

        // Logging.
        XCTAssertEqual(p1.firstAvailableLogEntryIndex(), 1)
        XCTAssertEqual(p1.lastAvailableLogEntryIndex(), 2)
        XCTAssertEqual(p1.logEntry(atIndex: 1), "1+1=2")
        XCTAssertEqual(p1.logEntry(atIndex: 2), "2/0=DIV BY ZERO")

        p1.clearLog()
        XCTAssertEqual(p1.firstAvailableLogEntryIndex(), 0)
        XCTAssertEqual(p1.lastAvailableLogEntryIndex(), 0)

        // Save Penta100 to file system.
        let p1Raw:Penta1000Raw = p1.raw()
        let p1RawData = Data(bytes: p1Raw.buffer, count: p1Raw.bufferSize)
        let dirURL:URL? = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
        let fileURL:URL? = dirURL?.appendingPathComponent("penta1000.dat")
        XCTAssertNotNil(fileURL)

        if (fileURL != nil) {
            do {
                try p1RawData.write(to: fileURL!, options: .atomic)
            } catch {
                XCTAssertNotNil(nil, "Couldn't write file")
            }
        }

        // Read Penta1000 from file system.
        if (fileURL != nil) {
            var fileRawData: Data? = nil
            do {
                try fileRawData = Data(contentsOf: fileURL!)
            } catch {
                XCTAssertNotNil(nil, "Couldn't read file")
            }
            if (fileRawData != nil) {
                var fileRawBuffer:UnsafePointer<Int8>?
                fileRawData!.withUnsafeBytes {
                    fileRawBuffer = $0
                }
                let p1FromFile:Penta1000 = Penta1000(rawBuffer: fileRawBuffer!)
                XCTAssertEqual(p1FromFile.display(), "___         ")
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
