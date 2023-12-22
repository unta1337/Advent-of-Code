#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 200
#define LEN_LINE 200

#define LEN_QUEUE 300000

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

typedef struct {
    size_t row;
    size_t col;
} pos_t;

typedef struct {
    pos_t pos;
    size_t steps;
} frame_t;

typedef struct {
    frame_t frames[LEN_QUEUE];
    size_t begin;
    size_t end;
} queue_t;

void queue_push(queue_t* queue, frame_t frame) {
    queue->frames[queue->end] = frame;
    queue->end += 1;
    queue->end %= LEN_QUEUE;
}

void queue_pop(queue_t* queue) {
    queue->begin += 1;
    queue->begin %= LEN_QUEUE;
}

frame_t queue_front(queue_t* queue) {
    return queue->frames[queue->begin];
}

size_t queue_count(queue_t* queue) {
    size_t begin = queue->begin;
    size_t end = queue->end;

    if (begin > end) {
        end += LEN_QUEUE;
    }

    return end - begin;
}

size_t bfs(string_t* lines, size_t len_lines, pos_t begin, size_t step_limit) {
    queue_t q = { 0, };
    queue_push(&q, (frame_t){ .pos = begin, .steps = 1 });

    size_t visited[LEN_LINES][LEN_LINES] = { 0, };

    while (queue_count(&q) > 0) {
        frame_t curr = queue_front(&q);
        queue_pop(&q);

        pos_t pos = curr.pos;
        size_t steps = curr.steps;

        if (pos.row >= len_lines || pos.col >= lines[0].len) {
            continue;
        } else if (lines[pos.row].str[pos.col] == '#') {
            continue;
        } else if (steps > step_limit + 1) {
            continue;
        } else if (visited[pos.row][pos.col] == steps) {
            continue;
        }
        visited[pos.row][pos.col] = steps;

        for (size_t i = 0; i < 4; i++) {
            pos_t next = pos;
            next.row += dirs[i][0];
            next.col += dirs[i][1];

            queue_push(&q, (frame_t){ .pos = next, .steps = steps + 1 });
        }
    }

    size_t ret = 0;
    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            ret += visited[i][j] == step_limit + 1;
        }
    }

    return ret;
}

size_t part_one(string_t* lines, size_t len_lines) {
    pos_t begin = { 0, };

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            if (lines[i].str[j] == 'S') {
                begin.row = i;
                begin.col = j;
            }
        }
    }

    size_t answer = bfs(lines, len_lines, begin, 64);

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
