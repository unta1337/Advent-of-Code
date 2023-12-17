#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 2000
#define LEN_LINE 2000

#define LEN_QUE 1000000

const size_t NONE = (1LLU << 32) - 1;

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef enum { UP, DOWN, LEFT, RIGHT } dir_t;

int dr[4] = { -1, 1, 0, 0 };
int dc[4] = { 0, 0, -1, 1 };

size_t dir_lookup[128][5] = {
    ['|'] = { 2, UP, DOWN },
    ['-'] = { 2, LEFT, RIGHT },
    ['L'] = { 2, UP, RIGHT },
    ['J'] = { 2, UP, LEFT },
    ['7'] = { 2, DOWN, LEFT },
    ['F'] = { 2, DOWN, RIGHT },
    ['.'] = { 0, },
    ['S'] = { 4, UP, DOWN, LEFT, RIGHT }
};

typedef struct {
    size_t row;
    size_t col;
    size_t step_count;
    dir_t prev_dir;
} frame_t;

void frame_copy(frame_t* dest, frame_t* src) {
    memcpy(dest, src, sizeof(frame_t));
}

typedef struct {
    frame_t* arr;
    size_t capacity;
    size_t begin;
    size_t count;
} queue_t;

queue_t queue_create(size_t capacity) {
    queue_t q = {
        .arr = (frame_t*)calloc(capacity, sizeof(frame_t)),
        .capacity = capacity,
        .begin = 0,
        .count = 0
    };

    return q;
}

void queue_push(queue_t* ths, frame_t frame) {
    if (ths->count >= ths->capacity) {
        fprintf(stderr, "Error: buy more ram lollll!!!\n");
        exit(1);
    }

    size_t index = (ths->begin + ths->count) % ths->capacity;
    frame_copy(&ths->arr[index], &frame);

    ths->count++;
}

void queue_pop(queue_t* ths) {
    if (ths->count == 0) {
        fprintf(stderr, "Error: queue is already empty\n");
        exit(1);
    }

    ths->count--;
    ths->begin += 1;
    ths->begin %= ths->capacity;
}

frame_t queue_front(queue_t* ths) {
    if (ths->count == 0) {
        fprintf(stderr, "Error: queue is already empty\n");
        exit(1);
    }

    return ths->arr[ths->begin];
}

void mark(string_t* map, size_t row, size_t col, char fill) {
    if (row >= map->len || col >= map[row].len) {
        return;
    }
    char cell = map[row].str[col];
    if (cell != '.') {
        return;
    }

    map[row].str[col] = fill;

    mark(map, row - 1, col, fill);
    mark(map, row + 1, col, fill);
    mark(map, row, col - 1, fill);
    mark(map, row, col + 1, fill);
}

size_t part_one(string_t* lines, size_t len_lines) {
    frame_t begin = { 0, };
    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            if (lines[i].str[j] == 'S') {
                begin.row = i;
                begin.col = j;
            }
        }
    }

    size_t rows = len_lines;
    size_t cols = lines[0].len;

    size_t* visited = (size_t*)malloc(rows * cols * sizeof(size_t));
    for (size_t i = 0; i < rows * cols; i++) {
        visited[i] = NONE;
    }

    queue_t q = queue_create(LEN_QUE);
    queue_push(&q, begin);

    while (q.count > 0) {
        frame_t curr = queue_front(&q);
        queue_pop(&q);

        visited[curr.row * cols + curr.col] = curr.step_count;

        char curr_cell = lines[curr.row].str[curr.col];
        for (size_t i = 1; i <= dir_lookup[curr_cell][0]; i++) {
            size_t dir = dir_lookup[curr_cell][i];
            size_t r = curr.row + dr[dir];
            size_t c = curr.col + dc[dir];

            if (r >= rows || c >= cols) {
                continue;
            } else if (visited[r * cols + c] < visited[curr.row * cols + curr.col]) {
                continue;
            } else {
                switch (lines[r].str[c]) {
                case '|':
                    if (dir != UP && dir != DOWN) continue;
                    break;
                case '-':
                    if (dir != LEFT && dir != RIGHT) continue;
                    break;
                case 'L':
                    if (dir != DOWN && dir != LEFT) continue;
                    break;
                case 'J':
                    if (dir != DOWN && dir != RIGHT) continue;
                    break;
                case '7':
                    if (dir != UP && dir != RIGHT) continue;
                    break;
                case 'F':
                    if (dir != UP && dir != LEFT) continue;
                    break;
                case '.':
                    continue;
                case 'S':
                    break;
                }
            }

            queue_push(&q, (frame_t){ .row = r,
                                      .col = c,
                                      .step_count = curr.step_count + 1 });
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (visited[i * cols + j] != NONE) {
                answer = MAX(answer, visited[i * cols + j]);
            }
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    frame_t begin = { 0, };
    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            if (lines[i].str[j] == 'S') {
                begin.row = i;
                begin.col = j;
            }
        }
    }

    size_t rows = len_lines;
    size_t cols = lines[0].len;

    size_t* visited = (size_t*)malloc(rows * cols * sizeof(size_t));
    for (size_t i = 0; i < rows * cols; i++) {
        visited[i] = NONE;
    }

    queue_t q = queue_create(LEN_QUE);
    queue_push(&q, begin);

    while (q.count > 0) {
        frame_t curr = queue_front(&q);
        queue_pop(&q);

        visited[curr.row * cols + curr.col] = curr.step_count;

        char curr_cell = lines[curr.row].str[curr.col];

        for (size_t i = 1; i <= dir_lookup[curr_cell][0]; i++) {
            size_t dir = dir_lookup[curr_cell][i];
            size_t r = curr.row + dr[dir];
            size_t c = curr.col + dc[dir];

            if (r >= rows || c >= cols) {
                continue;
            } else if (visited[r * cols + c] < visited[curr.row * cols + curr.col]) {
                continue;
            } else {
                switch (lines[r].str[c]) {
                case '|':
                    if (dir != UP && dir != DOWN) continue;
                    break;
                case '-':
                    if (dir != LEFT && dir != RIGHT) continue;
                    break;
                case 'L':
                    if (dir != DOWN && dir != LEFT) continue;
                    break;
                case 'J':
                    if (dir != DOWN && dir != RIGHT) continue;
                    break;
                case '7':
                    if (dir != UP && dir != RIGHT) continue;
                    break;
                case 'F':
                    if (dir != UP && dir != LEFT) continue;
                    break;
                case '.':
                    continue;
                case 'S':
                    continue;
                }
            }

            queue_push(&q, (frame_t){ .row = r,
                                      .col = c,
                                      .step_count = curr.step_count + 1 });
        }
    }

    for (size_t i = 0; i <= rows; i++) {
        for (size_t j = 0; j <= cols; j++) {
            if (visited[i * cols + j] == NONE) {
                lines[i].str[j] = '.';
            }
        }
    }

    q = queue_create(LEN_QUE);
    queue_push(&q, begin);

    visited = (size_t*)malloc(rows * cols * sizeof(size_t));
    for (size_t i = 0; i < rows * cols; i++) {
        visited[i] = NONE;
    }

    while (q.count > 0) {
        frame_t curr = queue_front(&q);
        queue_pop(&q);

        visited[curr.row * cols + curr.col] = curr.step_count;

        char curr_cell = lines[curr.row].str[curr.col];
        bool skip = false;

        if (curr_cell != '.' && curr_cell != 'S') {
            switch (curr.prev_dir) {
            case UP:
                mark(lines, curr.row, curr.col - 1, 'I');
                mark(lines, curr.row, curr.col + 1, 'O');
                if (curr_cell == '7') {
                    mark(lines, curr.row - 1, curr.col, 'O');
                } else if (curr_cell == 'F') {
                    mark(lines, curr.row - 1, curr.col, 'I');
                }
                break;
            case DOWN:
                mark(lines, curr.row, curr.col + 1, 'I');
                mark(lines, curr.row, curr.col - 1, 'O');
                if (curr_cell == 'J') {
                    mark(lines, curr.row + 1, curr.col, 'I');
                } else if (curr_cell == 'L') {
                    mark(lines, curr.row + 1, curr.col, 'O');
                }
                break;
            case LEFT:
                mark(lines, curr.row + 1, curr.col, 'I');
                mark(lines, curr.row - 1, curr.col, 'O');
                if (curr_cell == 'J') {
                    mark(lines, curr.row, curr.col + 1, 'O');
                } else if (curr_cell == '7') {
                    mark(lines, curr.row, curr.col + 1, 'I');
                }
                break;
            case RIGHT:
                mark(lines, curr.row - 1, curr.col, 'I');
                mark(lines, curr.row + 1, curr.col, 'O');
                if (curr_cell == 'L') {
                    mark(lines, curr.row, curr.col - 1, 'I');
                } else if (curr_cell == 'F') {
                    mark(lines, curr.row, curr.col - 1, 'O');
                }
                break;
            }
        }

        for (size_t i = 1; i <= dir_lookup[curr_cell][0]; i++) {
            size_t dir = dir_lookup[curr_cell][i];
            size_t r = curr.row + dr[dir];
            size_t c = curr.col + dc[dir];

            if (r >= rows || c >= cols) {
                continue;
            } else if (visited[r * cols + c] < visited[curr.row * cols + curr.col]) {
                continue;
            } else if (curr_cell == 'S' && skip) {
                continue;
            } else {
                switch (lines[r].str[c]) {
                case '|':
                    if (dir != UP && dir != DOWN) continue;
                    break;
                case '-':
                    if (dir != LEFT && dir != RIGHT) continue;
                    break;
                case 'L':
                    if (dir != DOWN && dir != LEFT) continue;
                    break;
                case 'J':
                    if (dir != DOWN && dir != RIGHT) continue;
                    break;
                case '7':
                    if (dir != UP && dir != RIGHT) continue;
                    break;
                case 'F':
                    if (dir != UP && dir != LEFT) continue;
                    break;
                case '.':
                    continue;
                case 'S':
                    continue;
                }
            }
            skip = true;

            queue_push(&q, (frame_t){ .row = r,
                                      .col = c,
                                      .step_count = curr.step_count + 1,
                                      .prev_dir = dir });
        }
    }

    size_t I_count = 0;
    size_t O_count = 0;

    size_t answer = 0;
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            switch (lines[i].str[j]) {
            case 'I': I_count++; break;
            case 'O': O_count++; break;
            }
        }
    }

    return MIN(I_count, O_count);
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
