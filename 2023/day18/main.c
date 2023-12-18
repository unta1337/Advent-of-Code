#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 700
#define LEN_LINE 50

#define GRID_LEN 600

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef enum {
    LEFT,
    DOWN,
    UP,
    RIGHT
} dir_t;

int dirs[][2] = {
    [LEFT] = { 0, -1 },
    [DOWN] = { 1, 0 },
    [UP] = { -1, 0 },
    [RIGHT] = { 0, 1 }
};
size_t dir_count = sizeof(dirs) / sizeof(dirs[0]);

dir_t get_dir(char d) {
    switch (d) {
    case 'L': return LEFT;
    case 'D': return DOWN;
    case 'U': return UP;
    case 'R': return RIGHT;
    }

    return LEFT;
}

bool is_inside(char grid[GRID_LEN][GRID_LEN], size_t row, size_t col) {
    size_t r = row;
    size_t c = col;

    if (grid[r][c] == '#') {
        return false;
    }

    size_t count = 0;
    while (c < GRID_LEN) {
        if (grid[r][c] == '#') {
            dir_t begin_dir = DOWN;
            if (r == 0) {
                begin_dir = DOWN;
            } else if (r == GRID_LEN - 1) {
                begin_dir = UP;
            } else {
                begin_dir = grid[r + 1][c] == '#' ? DOWN : UP;
            }
            size_t temp = 0;

            while (c < GRID_LEN && grid[r][c] == '#') {
                c--;
                temp++;
            }

            if (temp == 1) {
                count++;
            } else {
                dir_t end_dir = DOWN;
                if (r == 0) {
                    end_dir = DOWN;
                } else if (r == GRID_LEN - 1) {
                    end_dir = UP;
                } else {
                    end_dir = grid[r + 1][c + 1] == '#' ? DOWN : UP;
                }

                count += begin_dir != end_dir;
            }
        } else {
            c--;
        }
    }

    return count % 2 == 0 ? false : true;
}

size_t part_one(string_t* lines, size_t len_lines) {
    char grid[GRID_LEN][GRID_LEN] = { 0, };

    size_t r = GRID_LEN / 2;
    size_t c = GRID_LEN / 2;
    grid[r][c] = '#';

    for (size_t i = 0; i < len_lines; i++) {
        char* dir_str = strtok(lines[i].str, " ");
        dir_t dir = get_dir(*dir_str);

        char* count_str = strtok(NULL, " ");
        size_t count = strtoll(count_str, NULL, 10);

        for (size_t j = 0; j < count; j++) {
            r += dirs[dir][0];
            c += dirs[dir][1];
            grid[r][c] = '#';
        }
    }

    for (size_t i = 0; i < GRID_LEN; i++) {
        for (size_t j = 0; j < GRID_LEN; j++) {
            if (is_inside(grid, i, j)) {
                grid[i][j] = '@';
            }
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < GRID_LEN; i++) {
        for (size_t j = 0; j < GRID_LEN; j++) {
            answer += grid[i][j] == '#' || grid[i][j] == '@';
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    for (size_t i = 0; i < len_lines; i++) {
        printf("%4zu: \"%s\" (%zu)\n", i, lines[i].str, lines[i].len);
    }

    return 0;
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
