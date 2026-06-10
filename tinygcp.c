/*

TinyGCP: Lightweight G-code parser
Copyright (C) 2026 Hugo Barriga

*/

#include "tinygcp.h"

void dump_block(FILE *stream, Block *b)
{
	fprintf(stream, "block at %p\n{\n", (void *) b);
	fprintf(stream, "\tblock_del = %s\n", (b->block_del ? "true" : "false"));
	fprintf(stream, "\tpercent = %s\n", (b->percent ? "true" : "false"));
	fprintf(stream, "\tcomments = `%s`\n", b->comments);
	fprintf(stream, "\tlast_comment = `%s`\n", b->last_comment);
	fprintf(stream, "\tg_code = %d\n", b->g_code);
	fprintf(stream, "\tm_code = %d\n", b->m_code);
	fprintf(stream, "\t{f: %.3f, x: %.3f, y: %.3f, z: %.3f}\n", b->f, b->x, b->y, b->z);
	fprintf(stream, "}\n");
}

static inline int16_t is_word(char letter)
{
	const char valid_words[WORDS_COUNT] = {'F', 'G', 'M', 'X', 'Y', 'Z'};

	bool is_alpha = ((letter | 0x20) - 'a') <= ('z' - 'a');
	letter = letter & ~(is_alpha * 0x20);  // Be case-insensitive

	for (size_t i = 0; i < WORDS_COUNT; i++)
		if (letter == valid_words[i]) return i;

	return -1;
}

static inline double parse_coord(const char *coord, char **end)
{
	const char *p = coord;
	if (*p == '+' || *p == '-') p++;
	while ((*p >= '0' && *p <= '9') || *p == '.') p++;
	*end = (char *)p;

	char tmp[32];
	size_t len = p - coord;
	memcpy(tmp, coord, len);
	tmp[len] = '\0';
	return strtod(tmp, NULL);
}

void parse_line(Block *b, const char *line)
{
    b->block_del = false;
    b->percent   = false;
    b->g_code    = -1;
    b->m_code    = -1;

	char buf[MAX_LINE_SIZE] = {0};

	size_t line_len = 0;
	size_t comments_len = 0;
	size_t src_len = strlen(line);
	if (src_len > MAX_LINE_SIZE - 1)
		src_len = MAX_LINE_SIZE - 1;

	for (size_t i = 0; i < src_len; i++) {
		char c = line[i];
		if (c == '\n' || c == '\r') break;
		buf[i] = c;
		line_len++;
	}

	bool in_paren = false;
	bool in_semi = false;
	size_t dst = 0;

	for (size_t i = 0; i < line_len; i++) {
		char c = buf[i];

		if (!in_semi && c == '(') in_paren = true;
		if (!in_paren && c == ';') in_semi = true;

		if (in_paren || in_semi) {
			b->comments[comments_len++] = c;

			if (c == ')') {
				in_paren = false;
				in_semi = false;
			}

			continue;
		}

		if (c == ')') continue;

		if (c != ' ' && c != '\t') buf[dst++] = c;
	}

	buf[dst] = '\0';
	line_len = dst;
	memset(b->comments + comments_len, 0, MAX_LINE_SIZE - comments_len);
	b->last_comment = b->comments;

	for (size_t i = 0; i < comments_len; i++) {
		if (b->comments[i] == '(')
			b->last_comment = b->comments + i;
		else if (b->comments[i] == ';') {
			b->last_comment = b->comments + i;
			break;
		}
	}

	b->block_del = (buf[0] == '/');
	b->percent   = (buf[0] == '%');

	if (!b->block_del && !b->percent) {
		char *end;
		size_t i = 0;
		while (i < line_len) {
			int16_t word = is_word(buf[i++]);
			end = &buf[i];
			switch (word) {
				case G_CODE: b->g_code = strtol(&buf[i], &end, 10); break;
				case M_CODE: b->m_code = strtol(&buf[i], &end, 10); break;
				case FEED:   b->f = parse_coord(&buf[i], &end); break;
				case X_AXIS: b->x = parse_coord(&buf[i], &end); break;
				case Y_AXIS: b->y = parse_coord(&buf[i], &end); break;
				case Z_AXIS: b->z = parse_coord(&buf[i], &end); break;
				default: break;
			}

			i = end - buf;
		}
	}
}
