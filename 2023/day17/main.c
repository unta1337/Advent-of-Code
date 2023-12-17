#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 200
#define LEN_LINE 200

const size_t NONE =  1 << 30;

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

int dirs[4][2] = {
    [LEFT] = { 0, -1 },
    [DOWN] = { 1, 0 },
    [UP] = { -1, 0 },
    [RIGHT] = { 0, 1 }
};

typedef struct {
    size_t index;
    size_t next[16];
    size_t next_cost[16];
    size_t next_count;
} node_t;

size_t part_one(string_t* lines, size_t len_lines) {
    size_t rows = len_lines;
    size_t cols = lines[0].len;
    size_t count = rows * cols * 4;

    node_t* nodes = (node_t*)calloc(count, sizeof(node_t));

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            for (size_t d = 0; d < 4; d++) {
                size_t index = i * cols * 4 + j * 4 + d;
                node_t* curr = &nodes[index];
                curr->index = index;

                size_t next_cost = 0;
                for (size_t k = 1; k <= 3; k++) {
                    size_t r = i + k * dirs[d][0];
                    size_t c = j + k * dirs[d][1];

                    if (r < rows && c < cols) {
                        next_cost += lines[r].str[c] - '0';
                        curr->next[curr->next_count] = r * cols * 4 + c * 4 + d;
                        curr->next_cost[curr->next_count] = next_cost;
                        curr->next_count++;
                    }
                }

                for (size_t dd = 0; dd < 4; dd++) {
                    if (dd != d) {
                        curr->next[curr->next_count] = i * cols * 4 + j * 4 + dd;
                        curr->next_cost[curr->next_count] = 0;
                        curr->next_count++;
                    }
                }
            }
        }
    }

    for (size_t r = 0; r < rows; r++) {
        for (size_t c = 0; c < rows; c++) {
            printf("(");
            for (size_t d = 0; d < 4; d++) {
                printf("%zu ", nodes[r * cols * 4 + c * 4 + d].next_count);
            }
            printf(") ");
        }
        printf("\n");
    }

    for (size_t d = 0; d < 4; d++) {
        size_t begin_index = 0 * rows * 4 + 0 * 4 + d;
        node_t* begin = &nodes[begin_index];

        size_t* cost = (size_t*)malloc(count * sizeof(size_t));
        for (size_t i = 0; i < count; i++) {
            cost[i] = NONE;
        }
        for (size_t i = 0; i < begin->next_count; i++) {
            cost[begin->next[i]] = begin->next_cost[i];
        }
        cost[begin_index] = 0;

        bool* visited = (bool*)calloc(count, sizeof(bool));
        visited[begin_index] = true;

        for (size_t i = 0; i < count - 2; i++) {
            size_t prev_min = NONE;
            size_t waypoint_index = 0;

            for (size_t j = 0; j < count; j++) {
                if (!visited[j] && cost[j] < prev_min) {
                    prev_min = cost[j];
                    waypoint_index = j;
                }
            }

            visited[waypoint_index] = true;

            node_t* waypoint = &nodes[waypoint_index];
            for (size_t j = 0; j < waypoint->next_count; j++) {
                size_t next_index = waypoint->next[j];
                size_t next_cost = waypoint->next_cost[j];

                if (!visited[next_index]) {
                    if (cost[waypoint_index] + next_cost < cost[next_index]) {
                        cost[next_index] = cost[waypoint_index] + next_cost;
                    }
                }
            }
        }

        for (size_t dd = 0; dd < 4; dd++) {
            size_t end_index = (rows - 1) * rows * 4 + (cols - 1) * 4 + dd;

            printf("%zu -> %zu: %zu\n", d, dd, cost[end_index]);
        }
    }

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
