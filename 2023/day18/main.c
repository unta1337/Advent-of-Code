#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define ABS(x) ((x) > (x) ? (x) : -(x))
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 700
#define LEN_LINE 50

#define GRID_LEN 600

typedef struct {
    char* str;
    long long len;
} string_t;

typedef enum {
    RIGHT,
    DOWN,
    LEFT,
    UP
} dir_t;

long long dirs[][2] = {
    [LEFT] = { 0, -1 },
    [DOWN] = { 1, 0 },
    [UP] = { -1, 0 },
    [RIGHT] = { 0, 1 }
};
long long dir_count = sizeof(dirs) / sizeof(dirs[0]);

dir_t get_dir(char d) {
    switch (d) {
    case 'L': return LEFT;
    case 'D': return DOWN;
    case 'U': return UP;
    case 'R': return RIGHT;
    }

    return LEFT;
}

typedef struct {
    long long row;
    long long col;
} pos_t;

bool is_inside(char grid[GRID_LEN][GRID_LEN], long long row, long long col) {
    long long r = row;
    long long c = col;

    if (grid[r][c] == '#') {
        return false;
    }

    long long count = 0;
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
            long long temp = 0;

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

long long part_one(string_t* lines, long long len_lines) {
    char grid[GRID_LEN][GRID_LEN] = { 0, };

    long long r = GRID_LEN / 2;
    long long c = GRID_LEN / 2;
    grid[r][c] = '#';

    for (long long i = 0; i < len_lines; i++) {
        char* dir_str = strtok(lines[i].str, " ");
        dir_t dir = get_dir(*dir_str);

        char* count_str = strtok(NULL, " ");
        long long count = strtoll(count_str, NULL, 10);

        for (long long j = 0; j < count; j++) {
            r += dirs[dir][0];
            c += dirs[dir][1];
            grid[r][c] = '#';
        }
    }

    for (long long i = 0; i < GRID_LEN; i++) {
        for (long long j = 0; j < GRID_LEN; j++) {
            if (is_inside(grid, i, j)) {
                grid[i][j] = '@';
            }
        }
    }

    long long answer = 0;
    for (long long i = 0; i < GRID_LEN; i++) {
        for (long long j = 0; j < GRID_LEN; j++) {
            answer += grid[i][j] == '#' || grid[i][j] == '@';
        }
    }

    return answer;
}

long long part_two(string_t* lines, long long len_lines) {
    pos_t points[2 * LEN_LINES] = { 0, };
    long long point_count = 0;

    long long r = GRID_LEN / 2;
    long long c = GRID_LEN / 2;

    for (long long i = 0; i < len_lines; i++) {
        strtok(lines[i].str, "#");
        char* token = strtok(NULL, ")");

        long long dir = token[5] - '0';
        token[5] = '\0';

        long long count = strtoll(token, NULL, 16);

        points[point_count].row = r;
        points[point_count].col = c;
        point_count++;

        r += count * dirs[dir][0];
        c += count * dirs[dir][1];
    }

    long long border_count = 0;
    long long area = 0;

    for (long long i = 0; i < point_count; i++) {
        pos_t* curr = &points[i];
        pos_t* next = &points[(i + 1) % point_count];

        long long begin = 0;
        long long end = 0;

        if (curr->row == next->row) {
            begin = MIN(curr->col, next->col);
            end = MAX(curr->col, next->col);
        } else {
            begin = MIN(curr->row, next->row);
            end = MAX(curr->row, next->row);
        }

        border_count += end - begin;

        area += curr->row * next->col - curr->col * next->row;
    }
    area = ABS(area) / 2;

    long long inner = area - border_count / 2 + 1;
    long long answer = inner + border_count;

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
    for (long long i = 0; i < LEN_LINES; i++) {
        lines[i].str = (char*)calloc(LEN_LINE, sizeof(char));
    }
    long long line_count = 0;

    // load input file into buffer line-by-line.
    for (line_count = 0; fgets(lines[line_count].str, LEN_LINE, fp) != NULL; line_count++) {
        // remove line feed character.
        lines[line_count].len = strlen(lines[line_count].str);
        lines[line_count].str[--lines[line_count].len] = '\0';
    }

    // print answer.
#ifndef PART_TWO
    printf("%lld\n", part_one(lines, line_count));
#else
    printf("%lld\n", part_two(lines, line_count));
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
