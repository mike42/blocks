Blocks [![Build Status](https://travis-ci.org/mike42/blocks.svg?branch=master)](https://travis-ci.org/mike42/blocks)
================
This is a simple text-based tetris clone for Linux.

Compiling
--------------
You will need ncurses to compile this, and doxygen to generate the documentation. On Debian:

        apt-get install libncurses5-dev doxygen

And then

        make && make doc
        ./bin/blocks

Controls
-------------
* Move: Right, down, left
* Rotate: Up
* Drop: Spacebar
* Quit: q

