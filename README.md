# TinyGCP

## Overview

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

## Usage Example

The following example takes G-code blocks from `stdin`, parses them one by one, and updates the `Block` structure. Parsed parameters can then be accessed directly through the structure fields (e.g. `b.x`). For each parsed block, the example dumps the complete structure to `stderr`.

```c
#include <stdio.h>
#include <tinygcp.h>

int main(void)
{
	Block b = {0};
	char buf[MAX_LINE_SIZE];

    while (fgets(buf, sizeof(buf), stdin) != NULL) {
		parse_line(&b, buf);
		dump_block(stderr, &b);
    }

	return 0;
}
```

Given a file `cam_sw_output.gcode`, with contents:
```text
%

G1 X5.5 Y5.5 Z2.0 F1200
G1 X1 Y1 Z1 F1000 ; inline comment

%
```

Running the example with said file redirected to its input:
```sh
./example < cam_sw_output.gcode
```

Results in the following output (thanks to `dump_block(FILE *, Block *)`):
```text
block at 0x7ffcea5ec040
{
	block_del = false
	percent = true
	comments = ``
	last_comment = ``
	g_code = -1
	m_code = -1
	{f: 0.000, x: 0.000, y: 0.000, z: 0.000}
}
block at 0x7ffcea5ec040
{
	block_del = false
	percent = false
	comments = ``
	last_comment = ``
	g_code = 1
	m_code = -1
	{f: 1200.000, x: 5.500, y: 5.500, z: 2.000}
}
block at 0x7ffcea5ec040
{
	block_del = false
	percent = false
	comments = `; inline comment`
	last_comment = `; inline comment`
	g_code = 1
	m_code = -1
	{f: 1000.000, x: 1.000, y: 1.000, z: 1.000}
}
block at 0x7ffcea5ec040
{
	block_del = false
	percent = true
	comments = ``
	last_comment = ``
	g_code = -1
	m_code = -1
	{f: 1000.000, x: 0.000, y: 0.000, z: 0.000}
}
```

## G-code Support

### Words

| Word | Member | Description |
|------|--------|-------------|
| G | `.g` | Active G command |
| M | `.m` | Active M command |
| X | `.x` | X coordinate |
| Y | `.y` | Y coordinate |
| Z | `.z` | Z coordinate |
| F | `.f` | Feed rate |

### Comments

Comment blocks `(those between parentheses)` and in-line comments `; those trailing the block`. Both kinds of comments are stored on `.comments` structure member.  As sometimes comments are used to trigger some action, such as `(print, ...)` or `(debug, ...)`. If there were several comments on the same block, only the last one would be interpreted. Thus, the last found comment will be pointed to by `.last_comment`.

### Other Symbols

- A leading slash `/`, which disables the whole block, will set `.block_del` to `true` if found.
- A percent symbol `%` being the first character of a block will, set `.percent` to `true` if found. Often used to mark the start and end of a G-code program.
