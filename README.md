# Pentatronics 1000

Family of emulators for the classic Pentatronics 1000 calculator.

The engine itself is written in C (see **engine** folder) and can be used to write emulators for different platforms.

Currently, emulators for the following platforms have been implemented:
- iOS / Swift
- MacOS / C (CLI)
- MacOS / Python

## Writing an emulator

Writing a Pentatronics 1000 emulator can be an excellent exercise to learn, or re-learn, a platform or language. It has the benefits of using many fundamentals in a relatively simple project:
- user interface - simple or complex
- threading - to display animations
- file access - to persist the state
- string manipulation - for logging

If the language of choice is not C or a superset, such as C++, the first step is to interface with the C engine. The specific mechanism depends on the platform. See the Swift and Python projects for examples.

The second step is to write an emulator that listens to user events and updates the display. See **mini.c** in the CLI project.

More complex projects, for example **full.c** in the CLI project, support animation, logging and persistence of the state.
