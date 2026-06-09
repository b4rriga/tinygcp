#include <stdio.h>
#include <tinygcp.h>

int main(void)
{
	Block b = {0};
	char buf[MAX_LINE_SIZE];

	size_t i = 1;
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
		parse_line(&b, buf);
		dump_block(stderr, &b);
		// printf("! Line %zu\n", i++);
    }

	return 0;
}
