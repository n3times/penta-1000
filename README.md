# Pentatronics 1000

Family of emulators for the classic Pentatronics 1000 calculator.

The [engine](https://github.com/n3times/penta-1000/engine) itself is written in C and can be used to write emulators for different platforms.

Currently, emulators for the following platforms have been implemented:
- [iOS/Swift](https://github.com/n3times/penta-1000/ios)
- [C/CLI](https://github.com/n3times/penta-1000/cli)
- [Python](https://github.com/n3times/penta-1000/python)

## Writing an emulator

Writing a Pentatronics 1000 emulator can be an excellent exercise to learn, or revisit, a platform or language. It has the benefits of using several fundamentals in a relatively simple project:
- user interface - simple or complex
- threading - to display animations
- file access - to persist the state
- string manipulation - for logging

If the language of choice is not C or a superset, such as C++, the first step is to interface with the C engine. The specific mechanism depends on the platform. See for example the [interface for Python](https://github.com/n3times/penta-1000/python/penta1000_engine.py).

The second step is to write an emulator that listens to user events and updates the display. See [mini.c](https://github.com/n3times/penta-1000/cli/mini.c) for example.

More complex projects, for example [full.c](https://github.com/n3times/penta-1000/cli/full.c), support animation, logging and persistence of the state.
