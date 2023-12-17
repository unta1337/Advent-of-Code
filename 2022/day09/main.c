#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ABS(x) ((x) > 0 ? (x) : -(x))

#define LEN_LINES 2000
#define LEN_LINE 2000

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

typedef struct {
    size_t row;
    size_t col;
} pos_t;

typedef struct {
    pos_t start;
    pos_t head_prev;
    pos_t head;
    pos_t tail;
    size_t rows;
    size_t cols;
} bridge_t;

typedef struct {
    pos_t start;
    pos_t segs[LEN_LINE];
    size_t seg_count;
    size_t rows;
    size_t cols;
} seg_bridge_t;

void move_head(bridge_t* bridge, char dir) {
    bridge->head_prev = bridge->head;

    switch (dir) {
    case 'U':
        bridge->head.row--;
        break;
    case 'D':
        bridge->head.row++;
        break;
    case 'L':
        bridge->head.col--;
        break;
    case 'R':
        bridge->head.col++;
        break;
    }
}

void draw_bridge(bridge_t* bridge) {
    for (size_t i = 0; i < bridge->rows; i++) {
        for (size_t j = 0; j < bridge->cols; j++) {
            char c = '.';

            if (i == bridge->start.row && j == bridge->start.col) {
                c = 's';
            }
            if (i == bridge->tail.row && j == bridge->tail.col) {
                c = 'T';
            }
            if (i == bridge->head.row && j == bridge->head.col) {
                c = 'H';
            }

            printf("%c", c);
        }
        printf("\n");
    }
}

void follow_tail(bridge_t* bridge, char dir) {
    if (bridge->tail.row - (bridge->tail.row != 0) <= bridge->head.row
        && bridge->head.row <= bridge->tail.row + 1) {
        if (bridge->tail.col - (bridge->tail.col != 0) <= bridge->head.col
            && bridge->head.col <= bridge->tail.col + 1) {
            return;
        }
    }

    bridge->tail = bridge->head_prev;
}

void draw_bridge_seg(seg_bridge_t* bridge) {
    for (size_t i = 0; i < bridge->rows; i++) {
        for (size_t j = 0; j < bridge->cols; j++) {
            char c = '.';

            if (i == bridge->start.row && j == bridge->start.col) {
                c = 's';
            }
            for (size_t k = bridge->seg_count - 1; k > 0; k--) {
                if (i == bridge->segs[k].row && j == bridge->segs[k].col) {
                    c = k + '0';
                }
            }
            if (i == bridge->segs[0].row && j == bridge->segs[0].col) {
                c = 'H';
            }

            printf("%c", c);
        }
        printf("\n");
    }
}

void move_head_seg(seg_bridge_t* bridge, char dir) {
    switch (dir) {
    case 'U':
        bridge->segs[0].row--;
        break;
    case 'D':
        bridge->segs[0].row++;
        break;
    case 'L':
        bridge->segs[0].col--;
        break;
    case 'R':
        bridge->segs[0].col++;
        break;
    }
}

void follow_tail_seg(seg_bridge_t* bridge, char dir) {
    for (size_t i = 1; i < bridge->seg_count; i++) {
        if (bridge->segs[i].row - (bridge->segs[i].row != 0) <= bridge->segs[i - 1].row
            && bridge->segs[i - 1].row <= bridge->segs[i].row + 1) {
            if (bridge->segs[i].col - (bridge->segs[i].col != 0) <= bridge->segs[i - 1].col
                && bridge->segs[i - 1].col <= bridge->segs[i].col + 1) {
                break;
            }
        }

        int dr = (int)bridge->segs[i - 1].row - bridge->segs[i].row;
        int dc = (int)bridge->segs[i - 1].col - bridge->segs[i].col;

        bridge->segs[i].row = (int)bridge->segs[i].row + dr / ABS(dr);
        bridge->segs[i].col = (int)bridge->segs[i].col + dc / ABS(dc);
    }
}

size_t part_one(string_t* lines, size_t len_lines) {
    size_t rows = LEN_LINES;
    size_t cols = LEN_LINE;

    size_t init_row = rows / 2;
    size_t init_col = cols / 2;

    bridge_t bridge = {
        .start = {
            .row = init_row,
            .col = init_col
        },
        .head_prev = {
            .row = init_row,
            .col = init_col
        },
        .head = {
            .row = init_row,
            .col = init_col
        },
        .tail = {
            .row = init_row,
            .col = init_col
        },
        .rows = rows,
        .cols = cols
    };

    bool visited[LEN_LINES][LEN_LINE] = { 0, };
    for (size_t i = 0; i < len_lines; i++) {
        char* token = strtok(lines[i].str, " ");
        char dir = *token;

        token = strtok(NULL, " ");
        size_t count = strtoll(token, NULL, 10);

        for (size_t i = 0; i < count; i++) {
            move_head(&bridge, dir);
            follow_tail(&bridge, dir);

            visited[bridge.tail.row][bridge.tail.col] = true;
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < bridge.rows; i++) {
        for (size_t j = 0; j < bridge.cols; j++) {
            answer += visited[i][j];
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    size_t rows = LEN_LINES;
    size_t cols = LEN_LINE;

    size_t init_row = rows / 2;
    size_t init_col = cols / 2;

    seg_bridge_t bridge = {
        .start = {
            .row = init_row,
            .col = init_col
        },
        .seg_count = 10,
        .rows = rows,
        .cols = cols
    };
    for (size_t i = 0; i < LEN_LINE; i++) {
        bridge.segs[i].row = init_row;
        bridge.segs[i].col = init_col;
    }

    bool visited[LEN_LINES][LEN_LINE] = { 0, };
    for (size_t i = 0; i < len_lines; i++) {
        char* token = strtok(lines[i].str, " ");
        char dir = *token;

        token = strtok(NULL, " ");
        size_t count = strtoll(token, NULL, 10);

        for (size_t i = 0; i < count; i++) {
            move_head_seg(&bridge, dir);
            follow_tail_seg(&bridge, dir);

            visited[bridge.segs[bridge.seg_count - 1].row][bridge.segs[bridge.seg_count - 1].col] = true;
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < bridge.rows; i++) {
        for (size_t j = 0; j < bridge.cols; j++) {
            answer += visited[i][j];
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
