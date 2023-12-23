// This not working with sample data anymore.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 200
#define LEN_LINE 200

#define LEN_QUEUE 300000

typedef struct {
    char* str;
    int len;
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
    int row;
    int col;
} pos_t;

typedef struct {
    pos_t pos;
    int steps_left;
} frame_t;

typedef struct {
    frame_t frames[LEN_QUEUE];
    int begin;
    int end;
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

int queue_count(queue_t* queue) {
    int begin = queue->begin;
    int end = queue->end;

    if (begin > end) {
        end += LEN_QUEUE;
    }

    return end - begin;
}

int bfs(string_t* lines, int len_lines, pos_t begin, int step_limit) {
    queue_t q = { 0, };
    queue_push(&q, (frame_t){ .pos = begin, .steps_left = step_limit });

    bool visited[LEN_LINES][LEN_LINES] = { 0, };
    int ret = 0;

    while (queue_count(&q) > 0) {
        frame_t curr = queue_front(&q);
        queue_pop(&q);

        pos_t pos = curr.pos;
        int steps_left = curr.steps_left;

        if (pos.row < 0 || pos.col < 0) {
            continue;
        } else if (pos.row >= len_lines || pos.col >= lines[0].len) {
            continue;
        } else if (lines[pos.row].str[pos.col] == '#') {
            continue;
        } else if (visited[pos.row][pos.col]) {
            continue;
        }
        visited[pos.row][pos.col] = true;

        if (steps_left % 2 == 0) {
            ret++;
        }
        if (steps_left == 0) {
            continue;
        }

        for (int i = 0; i < 4; i++) {
            pos_t next = pos;
            next.row += dirs[i][0];
            next.col += dirs[i][1];

            queue_push(&q, (frame_t){ .pos = next, .steps_left = steps_left - 1 });
        }
    }

    return ret;
}

int bfs_wrap(string_t* lines, int len_lines, pos_t begin, int step_limit) {
    queue_t q = { 0, };
    queue_push(&q, (frame_t){ .pos = begin, .steps_left = step_limit });

    bool visited[LEN_LINES][LEN_LINES] = { 0, };
    int ret = 0;

    while (queue_count(&q) > 0) {
        frame_t curr = queue_front(&q);
        queue_pop(&q);

        pos_t pos = curr.pos;
        int steps_left = curr.steps_left;

        if (pos.row < 0 || pos.col < 0) {
            continue;
        } else if (pos.row >= len_lines || pos.col >= lines[0].len) {
            continue;
        } else if (lines[pos.row].str[pos.col] == '#') {
            continue;
        } else if (visited[pos.row][pos.col]) {
            continue;
        }
        visited[pos.row][pos.col] = true;

        if (steps_left % 2 == 0) {
            ret++;
        }
        if (steps_left == 0) {
            continue;
        }

        for (int i = 0; i < 4; i++) {
            pos_t next = pos;
            next.row += dirs[i][0];
            next.col += dirs[i][1];

            queue_push(&q, (frame_t){ .pos = next, .steps_left = steps_left - 1 });
        }
    }

    return ret;
}

int part_one(string_t* lines, int len_lines) {
    pos_t begin = { 0, };

    for (int i = 0; i < len_lines; i++) {
        for (int j = 0; j < lines[i].len; j++) {
            if (lines[i].str[j] == 'S') {
                begin.row = i;
                begin.col = j;
            }
        }
    }

    int answer = bfs(lines, len_lines, begin, 64);

    return answer;
}

size_t part_two(string_t* lines, int len_lines) {
    size_t size = len_lines;
    size_t steps = 26501365;
    size_t radius = steps / size;

    size_t right_corner = bfs(lines, len_lines,
                           (pos_t){ .row = size / 2, .col = 0 },
                           steps - (size / 2 + 1) - ((radius - 1) * size));
    size_t left_corner = bfs(lines, len_lines,
                          (pos_t){ .row = size / 2, .col = size - 1 },
                          steps - (size / 2 + 1) - ((radius - 1) * size));
    size_t top_corner = bfs(lines, len_lines,
                         (pos_t){ .row = size - 1, .col = size / 2 },
                         steps - (size / 2 + 1) - ((radius - 1) * size));
    size_t bottom_corner = bfs(lines, len_lines,
                          (pos_t){ .row = 0, .col = size / 2 },
                          steps - (size / 2 + 1) - ((radius - 1) * size));

    size_t edge_top_right_out = bfs(lines, len_lines,
                                 (pos_t){ .row = size - 1, .col = 0 },
                                 steps - ((radius - 1) * size) - 2 * (size / 2 + 1));
    size_t edge_top_left_out = bfs(lines, len_lines,
                                (pos_t){ .row = size - 1, .col = size - 1 },
                                steps - ((radius - 1) * size) - 2 * (size / 2 + 1));
    size_t edge_bottom_right_out = bfs(lines, len_lines,
                                   (pos_t){ .row = 0, .col = 0 },
                                    steps - ((radius - 1) * size) - 2 * (size / 2 + 1));
    size_t edge_bottom_left_out = bfs(lines, len_lines,
                                   (pos_t){ .row = 0, .col = size - 1 },
                                   steps - ((radius - 1) * size) - 2 * (size / 2 + 1));
    size_t edge_top_right_in = bfs(lines, len_lines,
                                 (pos_t){ .row = size - 1, .col = 0 },
                                 steps - ((radius - 2) * size) - 2 * (size / 2 + 1));
    size_t edge_top_left_in = bfs(lines, len_lines,
                                (pos_t){ .row = size - 1, .col = size - 1 },
                                steps - ((radius - 2) * size) - 2 * (size / 2 + 1));
    size_t edge_bottom_right_in = bfs(lines, len_lines,
                                   (pos_t){ .row = 0, .col = 0 },
                                    steps - ((radius - 2) * size) - 2 * (size / 2 + 1));
    size_t edge_bottom_left_in = bfs(lines, len_lines,
                                   (pos_t){ .row = 0, .col = size - 1 },
                                   steps - ((radius - 2) * size) - 2 * (size / 2 + 1));

    size_t odd_blocks = bfs(lines, len_lines,
                         (pos_t){ .row = 0, .col = 0 },
                         2 * size + 2023);
    size_t even_blocks = bfs(lines, len_lines,
                          (pos_t){ .row = 0, .col = 0 },
                          2 * size + 2024);

    size_t answer = right_corner + left_corner + top_corner + bottom_corner;
    answer += odd_blocks * (size_t)pow(radius - 1, 2);
    answer += even_blocks * (size_t)pow(radius, 2);
    answer += (edge_top_right_out + edge_top_left_out + edge_bottom_right_out + edge_bottom_left_out) * radius;
    answer += (edge_top_right_in + edge_top_left_in + edge_bottom_right_in + edge_bottom_left_in) * (radius - 1);

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
    for (int i = 0; i < LEN_LINES; i++) {
        lines[i].str = (char*)calloc(LEN_LINE, sizeof(char));
    }
    int line_count = 0;

    // load input file into buffer line-by-line.
    for (line_count = 0; fgets(lines[line_count].str, LEN_LINE, fp) != NULL; line_count++) {
        // remove line feed character.
        lines[line_count].len = strlen(lines[line_count].str);
        lines[line_count].str[--lines[line_count].len] = '\0';
    }

    // print answer.
#ifndef PART_TWO
    printf("%d\n", part_one(lines, line_count));
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
