#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 10
#define LEN_LINE 10100

#define GRID_ROWS 5000
#define GRID_COLS 7

#define COL_BEGIN 2

const size_t NONE = 1 << 30;

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    char rock[5][5];
    size_t rows;
    size_t cols;
} rock_t;

rock_t rocks[] = {
    {
        .rock = {
            "####"
        },
        .rows = 1,
        .cols = 4
    },
    {
        .rock = {
            ".#.",
            "###",
            ".#."
        },
        .rows = 3,
        .cols = 3
    },
    {
        .rock = {
            "..#",
            "..#",
            "###"
        },
        .rows = 3,
        .cols = 3
    },
    {
        .rock = {
            "#",
            "#",
            "#",
            "#"
        },
        .rows = 4,
        .cols = 1
    },
    {
        .rock = {
            "##",
            "##"
        },
        .rows = 2,
        .cols = 2
    }
};
size_t rock_count = sizeof(rocks) / sizeof(rocks[0]);

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

void put_rock(char grid[GRID_ROWS][GRID_COLS], size_t rock_index, size_t prev_row) {
    rock_t rock = rocks[rock_index];
    size_t row_begin = prev_row - 3 - rock.rows;

    for (size_t i = 0; i < rock.rows; i++) {
        for (size_t j = 0; j < rock.cols; j++) {
            if (rock.rock[i][j] == '#') {
                grid[row_begin + i][COL_BEGIN + j] = '@';
            }
        }
    }
}

bool can_move(char grid[GRID_ROWS][GRID_COLS], dir_t dir) {
    bool ret = true;
    for (size_t i = GRID_ROWS - 1; i < GRID_ROWS; i--) {
        for (size_t j = 0; j < GRID_COLS; j++) {
            if (grid[i][j] == '@') {
                size_t r = i + dirs[dir][0];
                size_t c = j + dirs[dir][1];

                ret &= (r < GRID_ROWS) && (c < GRID_COLS) && (grid[r][c] != '#');
            }
        }
    }

    return ret;
}

size_t move_rock(char grid[GRID_ROWS][GRID_COLS], dir_t dir) {
    bool move = can_move(grid, dir);

    if (move) {
        for (size_t i = GRID_ROWS - 1; i < GRID_ROWS; i--) {
            if (dir == RIGHT) {
                for (size_t j = GRID_COLS - 1; j < GRID_COLS; j--) {
                    if (grid[i][j] == '@') {
                        size_t r = i + dirs[dir][0];
                        size_t c = j + dirs[dir][1];

                        grid[r][c] = grid[i][j];
                        grid[i][j] = '\0';
                    }
                }
            } else {
                for (size_t j = 0; j < GRID_COLS; j++) {
                    if (grid[i][j] == '@') {
                        size_t r = i + dirs[dir][0];
                        size_t c = j + dirs[dir][1];

                        grid[r][c] = grid[i][j];
                        grid[i][j] = '\0';
                    }
                }
            }
        }
    } else if (!move && dir == DOWN) {
        size_t ret = NONE;
        for (size_t i = GRID_ROWS - 1; i < GRID_ROWS; i--) {
            for (size_t j = 0; j < GRID_COLS; j++) {
                if (grid[i][j] == '@') {
                    ret = i;
                    grid[i][j] = '#';
                }
            }
        }

        return ret;
    }

    return NONE;
}

void draw_grid(char grid[GRID_ROWS][GRID_COLS]) {
    for (size_t i = 0; i < GRID_ROWS; i++) {
        printf("|");
        for (size_t j = 0; j < GRID_COLS; j++) {
            printf("%c", grid[i][j] != '\0' ? grid[i][j] : '.');
        }
        printf("|\n");
    }
    printf("+");
    for (size_t i = 0; i < GRID_COLS; i++) {
        printf("-");
    }
    printf("+\n");
}

size_t part_one(string_t* lines, size_t len_lines) {
    char grid[GRID_ROWS][GRID_COLS] = { 0, };

    // draw_grid(grid);
    // sleep(1);

    size_t rock_index = 0;
    size_t prev_row = GRID_ROWS;
    size_t tall = 0;

    char* moves = lines[0].str;
    size_t move_count = lines[0].len;
    size_t move_index = 0;

    size_t count = 0;
    while (true) {
        // printf("done: %zu %zu %zu\n", count, prev_row, GRID_ROWS - prev_row);
        printf("%zu %zu\n", count, tall);

        if (prev_row != NONE) {
            tall = MAX(tall, GRID_ROWS - prev_row);
            count++;

            if (count == 20 + 1) {
                printf("done: %zu %zu\n", count, tall);
                break;
            }

            put_rock(grid, rock_index, prev_row);
            rock_index = (rock_index + 1) % rock_count;
            prev_row = NONE;
        } else {
            dir_t move = DOWN;
            switch (moves[move_index]) {
            case '<': move = LEFT; break;
            case '>': move = RIGHT; break;
            }
            move_index = (move_index + 1) % move_count;
            prev_row = move_rock(grid, move);

            // system("clear");
            // draw_grid(grid);
            // printf("%zu %zu\n", count, prev_row);
            // sleep(1);

            prev_row = move_rock(grid, DOWN);
        }

        // system("clear");
        // draw_grid(grid);
        // printf("%zu %zu\n", count, prev_row);
        // sleep(1);
    }

    draw_grid(grid);
    // printf("%zu %zu\n", count, prev_row);

    return 0;
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
