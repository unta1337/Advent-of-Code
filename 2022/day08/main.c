#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 200
#define LEN_LINE 200

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

size_t row_max(size_t trees[LEN_LINES][LEN_LINES], size_t row, size_t begin, size_t end) {
    size_t ret = 0;
    for (size_t i = begin; i <= end; i++) {
        ret = MAX(ret, trees[row][i]);
    }

    return ret;
}

size_t col_max(size_t trees[LEN_LINES][LEN_LINES], size_t col, size_t begin, size_t end) {
    size_t ret = 0;
    for (size_t i = begin; i <= end; i++) {
        ret = MAX(ret, trees[i][col]);
    }

    return ret;
}

size_t row_count(size_t trees[LEN_LINES][LEN_LINE], size_t row, size_t begin, size_t end, size_t curr, int dir) {
    if (dir == 1) {
        for (size_t i = begin; i <= end; i++) {
            if (trees[row][i] >= curr) {
                return i - begin + 1;
            }
        }
    } else {
        for (size_t i = end; i < LEN_LINE && i >= begin; i--) {
            if (trees[row][i] >= curr) {
                return end - i + 1;
            }
        }
    }

    return end - begin + 1;
}

size_t col_count(size_t trees[LEN_LINES][LEN_LINE], size_t col, size_t begin, size_t end, size_t curr, int dir) {
    if (dir == 1) {
        for (size_t i = begin; i <= end; i++) {
            if (trees[i][col] >= curr) {
                return i - begin + 1;
            }
        }
    } else {
        for (size_t i = end; i < LEN_LINES && i >= begin; i--) {
            if (trees[i][col] >= curr) {
                return end - i + 1;
            }
        }
    }

    return end - begin + 1;
}

size_t part_one(string_t* lines, size_t len_lines) {
    size_t trees[LEN_LINES][LEN_LINE] = { 0, };
    size_t tree_rows = len_lines;
    size_t tree_cols = lines[0].len;

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            trees[i][j] = lines[i].str[j] - '0';
        }
    }

    size_t answer = len_lines * 2 + (lines[0].len - 2) * 2;
    for (size_t i = 1; i < len_lines - 1; i++) {
        for (size_t j = 1; j < lines[i].len - 1; j++) {
            size_t up = col_max(trees, j, 0, i - 1);
            size_t down = col_max(trees, j, i + 1, len_lines - 1);
            size_t left = row_max(trees, i, 0, j - 1);
            size_t right = row_max(trees, i, j + 1, lines[i].len - 1);
            size_t predicate = MIN(up, MIN(down, MIN(left, right)));

            answer += predicate < trees[i][j];
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    size_t trees[LEN_LINES][LEN_LINE] = { 0, };
    size_t tree_rows = len_lines;
    size_t tree_cols = lines[0].len;

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            trees[i][j] = lines[i].str[j] - '0';
        }
    }

    size_t answer = 0;
    for (size_t i = 1; i < len_lines - 1; i++) {
        for (size_t j = 1; j < lines[i].len - 1; j++) {
            size_t up = col_count(trees, j, 0, i - 1, trees[i][j], -1);
            size_t down = col_count(trees, j, i + 1, len_lines - 1, trees[i][j], 1);
            size_t left = row_count(trees, i, 0, j - 1, trees[i][j], -1);
            size_t right = row_count(trees, i, j + 1, lines[i].len - 1, trees[i][j], 1);

            answer = MAX(answer, up * down * left * right);
        }
    }

    return answer;
}

void solve(const char* input_path) {
    // read input file.
    FILE* fp = fopen(input_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open input file \"%s\": %s.\n", input_path, strerror(errno));
        exit(errno);
    }

    string_t lines[LEN_LINES] = { 0, };
    size_t line_count = 0;

    // load input file into buffer line-by-line.
    for (line_count = 0; fgets(lines[line_count].str, LEN_LINE, fp) != NULL; line_count++) {
        // remove line feed character.
        lines[line_count].len = strlen(lines[line_count].str);
        lines[line_count].str[--lines[line_count].len] = '\0';
    }

    // print answer.
#ifndef PART_TWO
    printf("%zu\n", part_one(lines, line_count));
#else
    printf("%zu\n", part_two(lines, line_count));
#endif

    fclose(fp);
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        fprintf(stderr, "Error: input file path not provided.\n");
        return 1;
    }

    solve(argv[1]);

    return 0;
}
