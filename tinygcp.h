/*

TinyGCP: Lightweight G-code parser

Word support list is kept to a minimum. This means the library is limited to
M-codes, G-codes, 3 axes (X, Y, Z), and the feed rate as it stands.

Copyright (C) 2026 Hugo Barriga

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef TINYGCP_H
#define TINYGCP_H

#define MAX_LINE_SIZE 256

enum {
	FEED,
	G_CODE,
	M_CODE,
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
	WORDS_COUNT
};

typedef struct {
    char comments[MAX_LINE_SIZE];
    char *last_comment;
    bool block_del, percent;
	int16_t g_code,	m_code;
	double f, x, y, z;
} Block;

void dump_block(FILE *stream, Block *b);
void parse_line(Block *b, const char *line);

#endif // TINYGCP_H
