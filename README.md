# Pentatronics 1000

Family of emulators for the classic Pentatronics 1000 calculator.

The [engine](https://github.com/n3times/penta-1000/tree/master/app_engine) itself is written in C and can be used to write emulators for different platforms.

Currently, emulators for the following platforms have been implemented:
- [iOS/Swift](https://github.com/n3times/penta-1000/tree/master/ios)
- [C/CLI](https://github.com/n3times/penta-1000/tree/master/cli)
- [Python](https://github.com/n3times/penta-1000/tree/master/python)

## Writing an emulator

Note that writing a Pentatronics 1000 emulator can be an excellent exercise to learn, or revisit, a platform or language. It combines user interface, threading and other fundamentals in a relatively simple project.

If the language of choice is not C or a superset, such as C++, the first step is to interface with the C engine. The specific mechanism depends on the platform. See for example the [interface for Python](https://github.com/n3times/penta-1000/tree/master/python/penta1000_engine.py).

The second step is to write an emulator that listens to user events and updates the display. See [mini.c](https://github.com/n3times/penta-1000/tree/master/cli/mini.c) for example.

More complex projects, for example [full.c](https://github.com/n3times/penta-1000/tree/master/cli/full.c), support animation, logging and persistence of the state.
