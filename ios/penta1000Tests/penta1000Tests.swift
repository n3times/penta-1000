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
        let p1 = engine_new(seed: 0)

        XCTAssertEqual(engine_get_display(p1: p1), "PENTATRONICS")
        for c in "1+1=" {
            engine_press_key(p1: p1, c: c)
        }
        XCTAssertEqual(engine_get_display(p1: p1), "2")

        for c in "/0=" {
            engine_press_key(p1: p1, c: c)
        }
        XCTAssertEqual(engine_get_display(p1: p1), "DIV BY ZERO")

        engine_press_key(p1: p1, c: "g")
        XCTAssertEqual(engine_get_display(p1: p1), "> HI-LO GAME")

        // Animation.

        while engine_is_animating(p1: p1) {
            engine_advance_frame(p1: p1)
        }
        XCTAssertEqual(engine_get_display(p1: p1), "___         ")

        // Logging.

        XCTAssertEqual(engine_log_get_first_available_index(p1: p1), 1)
        XCTAssertEqual(engine_log_get_last_available_index(p1: p1), 2)
        XCTAssertEqual(engine_log_get_entry(p1: p1, index: 1), "1+1=2")
        XCTAssertEqual(engine_log_get_entry(p1: p1, index: 2), "2/0=DIV BY ZERO")
        engine_log_clear(p1: p1)
        XCTAssertEqual(engine_log_get_first_available_index(p1: p1), 0)
        XCTAssertEqual(engine_log_get_last_available_index(p1: p1), 0)

        // Save State.

        let dir = FileManager.default.urls(for: .documentDirectory, in: .userDomainMask).first
        let fileURL = dir!.appendingPathComponent("penta1000.dat")
        let size = engine_get_state_size(p1: p1)
        let state = engine_get_state(p1: p1)
        let data = Data(bytes: state, count: size)

        try data.write(to: fileURL, options: .atomic)
        engine_release_state(state: state);

        // Read State.

        let saved_data = try Data(contentsOf: fileURL)
        var p1_from_state: OpaquePointer?
        saved_data.withUnsafeBytes {
            p1_from_state = engine_new_from_state(state: $0)}
        XCTAssertEqual(engine_get_display(p1: p1_from_state), "___         ")

        // Clean up
        engine_release(p1: p1)
        engine_release(p1: p1_from_state)
    }

    func testPerformanceExample() throws {
        // This is an example of a performance test case.
        self.measure {
            // Put the code you want to measure the time of here.
        }
    }

}
