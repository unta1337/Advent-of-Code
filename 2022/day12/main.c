#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define ABS(x) ((x) > 0 ? (x) : -(x))

#define LEN_LINES 2000
#define LEN_LINE 2000

#define LEN_QUE 100000

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    char prev_height;
    size_t row;
    size_t col;
    size_t step_count;
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

size_t part_one(string_t* lines, size_t len_lines) {
    frame_t begin = { 0, };
    frame_t end = { 0, };
    begin.prev_height = 'a' - 1;

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            if (lines[i].str[j] == 'S') {
                begin.row = i;
                begin.col = j;
            } else if (lines[i].str[j] == 'E') {
                end.row = i;
                end.col = j;
            }
        }
    }

    lines[begin.row].str[begin.col] = 'a';
    lines[end.row].str[end.col] = 'z';

    size_t rows = len_lines;
    size_t cols = lines[0].len;

    queue_t q = queue_create(LEN_QUE);
    queue_push(&q, begin);
    bool** visited = (bool**)calloc(rows, sizeof(bool*));
    for (size_t i = 0; i < rows; i++) {
        visited[i] = (bool*)calloc(cols, sizeof(bool));
    }

    size_t answer = 0;
    while (q.count > 0) {
        frame_t curr = queue_front(&q);
        queue_pop(&q);

        if (curr.row >= rows || curr.col >= cols) {
            continue;
        }

        char curr_height = lines[curr.row].str[curr.col];
        if (curr_height - curr.prev_height > 1) {
            continue;
        }

        if (visited[curr.row][curr.col]) {
            continue;
        }
        visited[curr.row][curr.col] = true;

        if (curr.row == end.row && curr.col == end.col) {
            answer = curr.step_count;
            break;
        }

        int dr[4] = { -1, 1, 0, 0 };
        int dc[4] = { 0, 0, -1, 1 };

        for (size_t i = 0; i < 4; i++) {
            size_t r = curr.row + dr[i];
            size_t c = curr.col + dc[i];

            queue_push(&q, (frame_t){ .prev_height = curr_height,
                                      .row = r,
                                      .col = c,
                                      .step_count = curr.step_count + 1 });
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    frame_t* begins = (frame_t*)calloc(LEN_LINES * LEN_LINE, sizeof(frame_t));
    size_t begin_count = 0;

    frame_t end = { 0, };

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            if (lines[i].str[j] == 'S' || lines[i].str[j] == 'a') {
                lines[i].str[j] = 'a';

                begins[begin_count].row = i;
                begins[begin_count].col = j;
                begins[begin_count].prev_height = 'a';
                begin_count++;
            } else if (lines[i].str[j] == 'E') {
                lines[i].str[j] = 'z';
                end.row = i;
                end.col = j;
            }
        }
    }

    size_t rows = len_lines;
    size_t cols = lines[0].len;

    size_t answer = 1 << 32 - 1;
    for (size_t t = 0; t < begin_count; t++) {
        queue_t q = queue_create(LEN_QUE);
        queue_push(&q, begins[t]);
        bool** visited = (bool**)calloc(rows, sizeof(bool*));
        for (size_t i = 0; i < rows; i++) {
            visited[i] = (bool*)calloc(cols, sizeof(bool));
        }

        size_t res = 1 << 32 - 1;
        while (q.count > 0) {
            frame_t curr = queue_front(&q);
            queue_pop(&q);

            if (curr.row >= rows || curr.col >= cols) {
                continue;
            }

            char curr_height = lines[curr.row].str[curr.col];
            if (curr_height - curr.prev_height > 1) {
                continue;
            }

            if (visited[curr.row][curr.col]) {
                continue;
            }
            visited[curr.row][curr.col] = true;

            if (curr.row == end.row && curr.col == end.col) {
                res = curr.step_count;
                break;
            }

            int dr[4] = { -1, 1, 0, 0 };
            int dc[4] = { 0, 0, -1, 1 };

            for (size_t i = 0; i < 4; i++) {
                size_t r = curr.row + dr[i];
                size_t c = curr.col + dc[i];

                queue_push(&q, (frame_t){ .prev_height = curr_height,
                                          .row = r,
                                          .col = c,
                                          .step_count = curr.step_count + 1 });
            }
        }

        answer = MIN(answer, res);
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
