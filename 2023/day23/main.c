#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 200
#define LEN_LINE 200

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN
} dir_t;

int dirs[][2] = {
    [LEFT] = { 0, -1 },
    [RIGHT] = { 0, 1 },
    [UP] = { -1, 0 },
    [DOWN] = { 1, 0 }
};

size_t dir_lookup[128] = {
    ['<'] = LEFT,
    ['>'] = RIGHT,
    ['^'] = UP,
    ['v'] = DOWN
};

typedef struct {
    size_t row;
    size_t col;
} pos_t;

size_t dfs(string_t* map, size_t rows, bool visited[LEN_LINES][LEN_LINES],
         pos_t begin, pos_t end,
         size_t steps,
         bool skip_slope) {
    size_t ret = 0;
    size_t cols = map[0].len;

    if (begin.row >= rows || begin.col >= cols) {
        return 0;
    } else if (map[begin.row].str[begin.col] == '#') {
        return 0;
    } if (visited[begin.row][begin.col]) {
        return 0;
    }
    visited[begin.row][begin.col] = true;

    if (!skip_slope) {
        char c = map[begin.row].str[begin.col];
        pos_t next = begin;
        switch (c) {
        case '<': case '>': case '^': case 'v':
            next.row += dirs[dir_lookup[c]][0];
            next.col += dirs[dir_lookup[c]][1];

            size_t temp = dfs(map, rows, visited, next, end, steps + 1, skip_slope);
            ret = MAX(ret, temp);
            goto defer;
        }
    }

    if (begin.row == end.row && begin.col == end.col) {
        size_t temp = steps;
        ret = MAX(ret, temp);
        goto defer;
    }

    for (size_t i = 0; i < 4; i++) {
        pos_t next = begin;
        next.row += dirs[i][0];
        next.col += dirs[i][1];

        size_t temp = dfs(map, rows, visited, next, end, steps + 1, skip_slope);
        ret = MAX(ret, temp);
    }

defer:
    visited[begin.row][begin.col] = false;

    return ret;
}

size_t part_one(string_t* lines, size_t len_lines) {
    bool visited[LEN_LINES][LEN_LINES] = { 0, };
    pos_t begin = { .row = 0, .col = 1 };
    pos_t end = { .row = len_lines - 1, .col = lines[0].len - 2 };

    size_t answer = dfs(lines, len_lines, visited, begin, end, 0, false);

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    bool visited[LEN_LINES][LEN_LINES] = { 0, };
    pos_t begin = { .row = 0, .col = 1 };
    pos_t end = { .row = len_lines - 1, .col = lines[0].len - 2 };

    size_t answer = dfs(lines, len_lines, visited, begin, end, 0, true);

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
    for (size_t i = 0; i < LEN_LINES; i++) {
        lines[i].str = (char*)calloc(LEN_LINE, sizeof(char));
    }
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
