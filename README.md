*Update from 2020* -- I am no longer working on this, but it was the first program I ever wrote in C! So, this repo has a lot of sentimental value.

This is in active development, no stable release currently.
Currently is playable. Terminal resize support still needs to be added.

Run 'doxygen Doxyfile' to create documentation.
Run 'make' to compile the program. GCC > 4.7 must be installed as well as the ncurses library (for C11 compatibility). Code portability through the use of autoconf will be added later.

Play like any other minesweeper program. Use the arrow keys to move (vi movement keys also work) and space/enter to reveal a mine. Use "f" to flag mines. Color output will be automatically used if supported unless specified otherwise. Pass "--help" for more information.
