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

        XCTAssertEqual(engine_get_display(), "PENTATRONICS")
        for c in "1+1=" {
            engine_press_key(c: c)
        }
        XCTAssertEqual(engine_get_display(), "2")

        for c in "/0=" {
            engine_press_key(c: c)
        }
        XCTAssertEqual(engine_get_display(), "DIV BY ZERO")

        engine_press_key(c: "g")
        XCTAssertEqual(engine_get_display(), "> HI-LO GAME")

        // Animation.

        while engine_is_animating() {
            engine_advance_frame()
        }
        XCTAssertEqual(engine_get_display(), "___         ")

        // Logging.

        XCTAssertEqual(engine_log_get_first_available_index(), 1)
        XCTAssertEqual(engine_log_get_last_available_index(), 2)
        XCTAssertEqual(engine_log_get_entry(index: 1), "1+1=2")
        XCTAssertEqual(engine_log_get_entry(index: 2), "2/0=DIV BY ZERO")
        engine_log_clear()
        XCTAssertEqual(engine_log_get_first_available_index(), 0)
        XCTAssertEqual(engine_log_get_last_available_index(), 0)
    }

    func testPerformanceExample() throws {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }

}
