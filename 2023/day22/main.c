#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 2000
#define LEN_LINE 200

#define LEN_SUPPORT 20

#define LEN_QUEUE 1000

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    size_t x;
    size_t y;
    size_t z;
} pos_t;

typedef struct brick_t {
    size_t index;

    pos_t begin;
    pos_t end;

    struct brick_t* supports[LEN_SUPPORT];
    size_t support_count;

    struct brick_t* supported_by[LEN_SUPPORT];
    size_t supported_count;
} brick_t;

int comp_z(const void* p, const void* q) {
    size_t i = ((brick_t*)p)->begin.z;
    size_t j = ((brick_t*)q)->begin.z;

    return (i > j) - (i < j);
}

bool is_overlap(brick_t* p, brick_t* q) {
    bool x_condition = MAX(p->begin.x, q->begin.x) <= MIN(p->end.x, q->end.x);
    bool y_condition = MAX(p->begin.y, q->begin.y) <= MIN(p->end.y, q->end.y);

    return x_condition && y_condition;
}

size_t part_one(string_t* lines, size_t len_lines) {
    brick_t bricks[LEN_LINES] = { 0, };
    size_t brick_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        brick_t* brick = &bricks[brick_count++];
        brick->index = brick_count - 1;

        char* begin_str = strtok(lines[i].str, "~");
        char* end_str = strtok(NULL, "~");

        char* x_str = strtok(begin_str, ",");
        char* y_str = strtok(NULL, ",");
        char* z_str = strtok(NULL, ",");

        brick->begin.x = strtoll(x_str, NULL, 10);
        brick->begin.y = strtoll(y_str, NULL, 10);
        brick->begin.z = strtoll(z_str, NULL, 10);

        x_str = strtok(end_str, ",");
        y_str = strtok(NULL, ",");
        z_str = strtok(NULL, ",");

        brick->end.x = strtoll(x_str, NULL, 10);
        brick->end.y = strtoll(y_str, NULL, 10);
        brick->end.z = strtoll(z_str, NULL, 10);

        assert(brick->begin.x <= brick->end.x);
        assert(brick->begin.y <= brick->end.y);
        assert(brick->begin.z <= brick->end.z);
    }

    qsort(bricks, brick_count, sizeof(brick_t), comp_z);

    for (size_t i = 0; i < brick_count; i++) {
        brick_t* ths = &bricks[i];
        size_t floor = 1;

        for (size_t j = 0; j < i; j++) {
            brick_t* other = &bricks[j];

            if (is_overlap(ths, other)) {
                floor = MAX(floor, other->end.z + 1);
            }
        }

        size_t fall_dist = ths->begin.z - floor;
        ths->begin.z -= fall_dist;
        ths->end.z -= fall_dist;
    }

    qsort(bricks, brick_count, sizeof(brick_t), comp_z);

    for (size_t i = 0; i < brick_count; i++) {
        brick_t* ths = &bricks[i];
        for (size_t j = i + 1; j < brick_count; j++) {
            brick_t* other = &bricks[j];

            if (is_overlap(ths, other) && other->begin.z - ths->end.z == 1) {
                ths->supports[ths->support_count++] = other;
                other->supported_by[other->supported_count++] = ths;
            }
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < brick_count; i++) {
        brick_t* brick = &bricks[i];
        bool is_ok = true;

        for (size_t j = 0; j < brick->support_count; j++) {
            is_ok &= brick->supports[j]->supported_count > 1;
        }

        answer += is_ok;
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    brick_t bricks[LEN_LINES] = { 0, };
    size_t brick_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        brick_t* brick = &bricks[brick_count++];
        brick->index = brick_count - 1;

        char* begin_str = strtok(lines[i].str, "~");
        char* end_str = strtok(NULL, "~");

        char* x_str = strtok(begin_str, ",");
        char* y_str = strtok(NULL, ",");
        char* z_str = strtok(NULL, ",");

        brick->begin.x = strtoll(x_str, NULL, 10);
        brick->begin.y = strtoll(y_str, NULL, 10);
        brick->begin.z = strtoll(z_str, NULL, 10);

        x_str = strtok(end_str, ",");
        y_str = strtok(NULL, ",");
        z_str = strtok(NULL, ",");

        brick->end.x = strtoll(x_str, NULL, 10);
        brick->end.y = strtoll(y_str, NULL, 10);
        brick->end.z = strtoll(z_str, NULL, 10);

        assert(brick->begin.x <= brick->end.x);
        assert(brick->begin.y <= brick->end.y);
        assert(brick->begin.z <= brick->end.z);
    }

    qsort(bricks, brick_count, sizeof(brick_t), comp_z);

    for (size_t i = 0; i < brick_count; i++) {
        brick_t* ths = &bricks[i];
        size_t floor = 1;

        for (size_t j = 0; j < i; j++) {
            brick_t* other = &bricks[j];

            if (is_overlap(ths, other)) {
                floor = MAX(floor, other->end.z + 1);
            }
        }

        size_t fall_dist = ths->begin.z - floor;
        ths->begin.z -= fall_dist;
        ths->end.z -= fall_dist;
    }

    qsort(bricks, brick_count, sizeof(brick_t), comp_z);

    for (size_t i = 0; i < brick_count; i++) {
        brick_t* ths = &bricks[i];
        for (size_t j = i + 1; j < brick_count; j++) {
            brick_t* other = &bricks[j];

            if (is_overlap(ths, other) && other->begin.z - ths->end.z == 1) {
                ths->supports[ths->support_count++] = other;
                other->supported_by[other->supported_count++] = ths;
            }
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < brick_count; i++) {
        brick_t* q[LEN_QUEUE] = { 0, };
        size_t q_begin = 0;
        size_t q_size = 0;

        q[(q_begin + q_size++) % LEN_QUEUE] = &bricks[i];

        bool fallen[LEN_LINES] = { 0, };

        while (q_size > 0) {
            brick_t* brick = q[q_begin];
            q_begin += 1;
            q_begin %= LEN_QUEUE;
            q_size -= 1;

            if (fallen[brick->index]) {
                continue;
            }
            fallen[brick->index] = true;

            for (size_t j = 0; j < brick->support_count; j++) {
                brick_t* temp = brick->supports[j];
                bool should_check = true;

                for (size_t k = 0; k < temp->supported_count; k++) {
                    should_check &= fallen[temp->supported_by[k]->index];
                }

                if (should_check) {
                    q[(q_begin + q_size++) % LEN_QUEUE] = temp;
                }
            }
        }

        size_t res = 0;
        for (size_t i = 0; i < LEN_LINES; i++) {
            res += fallen[i];
        }
        res--;

        answer += res;
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
