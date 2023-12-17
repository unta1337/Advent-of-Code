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

typedef struct {
    int r;
    int c;
} vec_t;

typedef enum {
    LEFT,
    DOWN,
    UP,
    RIGHT
} dir_t;

vec_t dirs[4] = {
    { .r = 0, .c = -1 },
    { .r = 1, .c = 0 },
    { .r = -1, .c = 0 },
    { .r = 0, .c = 1 }
};

char symbols[4] = { '<', 'v', '^', '>' };

typedef struct {
    vec_t pos;
    dir_t dir;
} light_t;

void dfs(string_t* lines, size_t len_lines, light_t light, bool visited[LEN_LINES][LEN_LINE][4]) {
    if (light.pos.r >= len_lines || light.pos.c >= lines[0].len) {
        return;
    }

    if (visited[light.pos.r][light.pos.c][light.dir]) {
        return;
    }
    visited[light.pos.r][light.pos.c][light.dir] = true;

    light_t p = light;
    light_t q = light;

    switch (lines[light.pos.r].str[light.pos.c]) {
    case '|':
        if (light.dir == DOWN || light.dir == UP) {
            break;
        }

        p.dir = UP;
        q.dir = DOWN;

        break;
    case '-':
        if (light.dir == RIGHT || light.dir == LEFT) {
            break;
        }

        p.dir = LEFT;
        q.dir = RIGHT;

        break;
    case '/':
        switch (light.dir) {
        case LEFT: p.dir = DOWN; break;
        case DOWN: p.dir = LEFT; break;
        case UP: p.dir = RIGHT; break;
        case RIGHT: p.dir = UP; break;
        }
        break;
    case '\\':
        switch (light.dir) {
        case LEFT: p.dir = UP; break;
        case DOWN: p.dir = RIGHT; break;
        case UP: p.dir = LEFT; break;
        case RIGHT: p.dir = DOWN; break;
        }
        break;
    }

    p.pos.r += dirs[p.dir].r;
    p.pos.c += dirs[p.dir].c;

    q.pos.r += dirs[q.dir].r;
    q.pos.c += dirs[q.dir].c;

    dfs(lines, len_lines, p, visited);

    switch (lines[light.pos.r].str[light.pos.c]) {
    case '|': case '-':
        dfs(lines, len_lines, q, visited);
    }
}

size_t helper(string_t* lines, size_t len_lines, light_t begin, bool print) {
    bool visited[LEN_LINES][LEN_LINE][4] = { 0, };

    dfs(lines, len_lines, begin, visited);

    size_t answer = 0;
    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            for (size_t k = 0; k < 4; k++) {
                if (visited[i][j][k]) {
                    answer ++;
                    break;
                }
            }
        }
    }

    if (print) {
        for (size_t i = 0; i < len_lines; i++) {
            for (size_t j = 0; j < lines[i].len; j++) {
                char c = '.';

                size_t count = 0;
                for (size_t k = 0; k < 4; k++) {
                    if (visited[i][j][k]) {
                        count++;
                        c = symbols[k];
                    }
                }

                if (count > 1) {
                    c = count + '0';
                }

                if (lines[i].str[j] != '.') {
                    c = lines[i].str[j];
                }

                printf("%c", c);
            }
            printf("\n");
        }
    }

    return answer;
}

size_t part_one(string_t* lines, size_t len_lines) {
    light_t light = {
        .pos = {
            .r = 0,
            .c = 0
        },
        .dir = RIGHT
    };

    return helper(lines, len_lines, light, false);
}

size_t part_two(string_t* lines, size_t len_lines) {
    light_t light = { 0, };

    size_t answer = 0;

    for (size_t r = 0; r < len_lines; r++) {
        light.pos.r = r;

        light.pos.c = 0;
        light.dir = RIGHT;
        size_t from_left = helper(lines, len_lines, light, false);

        light.pos.c = lines[r].len - 1;
        light.dir = LEFT;
        size_t from_right = helper(lines, len_lines, light, false);

        answer = MAX(answer, MAX(from_left, from_right));
    }

    for (size_t c = 0; c < lines[0].len; c++) {
        light.pos.c = c;

        light.pos.r = 0;
        light.dir = DOWN;
        size_t from_left = helper(lines, len_lines, light, false);

        light.pos.r = len_lines - 1;
        light.dir = UP;
        size_t from_right = helper(lines, len_lines, light, false);

        answer = MAX(answer, MAX(from_left, from_right));
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
