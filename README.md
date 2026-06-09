# TinyGCP

## Description

A simple, extremely barebones G-code parser, implemented in **about 100 lines of code!**

As is, this library can parse through a sequence of G-code _blocks_ (meaning commands in G-code parlance) and extract:

- M-codes
- G-codes
- X, Y and Z axis position
- Machine feed rate
- Comments (if any)

Adding support for more words, such as additional axes, should be trivial for any C programmer.

TinyGCP may be used as a command prompt interpreter for a serial console link with a CNC machine, or it may be run in a loop, being able to interpret `.gcode` files. Both CRLF and NL line ending formats are supported.

## Installation

Build:
```sh
make
```

Install:
```sh
sudo make install
```

## Example

**TODO**
