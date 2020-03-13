# Command line interface for MacOS
Instructions: from this directory, build with "make" and run with "a.out".

To port to other systems, note that this CLI uses:
* pthreads to animate the display
* ANSI escape codes to modify the foreground and background colors of the display
* system calls such as "/bin/stty raw" (see main.c)
