#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 2000
#define LEN_LINE 2000

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    double x;
    double y;
    double z;
} vec_t;

void trim_left(char** str) {
    while (**str != '\0' && **str == ' ') {
        (*str)++;
    }
}

void trim_right(char** str) {
    int len = strlen(*str);
    while (len > 0 && (*str)[len - 1] == ' ') {
        (*str)[--len] = '\0';
    }
}

void trim(char** str) {
    trim_left(str);
    trim_right(str);
}

typedef struct {
    vec_t pos;
    vec_t vel;
} hail_t;

vec_t get_intersection(hail_t* p, hail_t* q) {
    double m1 = p->vel.y / p->vel.x;
    double a1 = p->pos.x;
    double b1 = p->pos.y;

    double m2 = q->vel.y / q->vel.x;
    double a2 = q->pos.x;
    double b2 = q->pos.y;

    double x = (m1 * a1- b1 - (m2 * a2 - b2)) / (m1 - m2);
    double y = m1 * x - m1 * a1 + b1;

    return (vec_t){
        .x = x,
        .y = y,
        .z = 0.0f
    };
}

size_t part_one(string_t* lines, size_t len_lines) {
    hail_t hails[LEN_LINES] = { 0, };
    size_t hail_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        hail_t* hail = &hails[hail_count++];

        char* pos_str = strtok(lines[i].str, "@");
        char* vel_str = strtok(NULL, "@");

        trim(&pos_str);
        trim(&vel_str);

        char* x_str = strtok(pos_str, ",");
        char* y_str = strtok(NULL, ",");
        char* z_str = strtok(NULL, ",");

        trim(&x_str);
        trim(&y_str);
        trim(&z_str);

        hail->pos.x = strtod(x_str, NULL);
        hail->pos.y = strtod(y_str, NULL);
        hail->pos.z = strtod(z_str, NULL);

        x_str = strtok(vel_str, ",");
        y_str = strtok(NULL, ",");
        z_str = strtok(NULL, ",");

        trim(&x_str);
        trim(&y_str);
        trim(&z_str);

        hail->vel.x = strtod(x_str, NULL);
        hail->vel.y = strtod(y_str, NULL);
        hail->vel.z = strtod(z_str, NULL);
    }

    for (size_t i = 0; i < hail_count; i++) {
        hail_t* ths = &hails[i];

        for (size_t j = i + 1; j < hail_count; j++) {
            hail_t* other = &hails[j];

            vec_t inter = get_intersection(ths, other);

            printf("A: %lf, %lf, %lf @ %lf, %lf, %lf\n", ths->pos.x, ths->pos.y, ths->pos.z,
                                                         ths->vel.x, ths->vel.y, ths->vel.z);
            printf("B: %lf, %lf, %lf @ %lf, %lf, %lf\n", other->pos.x, other->pos.y, other->pos.z,
                                                         other->vel.x, other->vel.y, other->vel.z);
            printf("I: %lf, %lf, %lf\n", inter.x, inter.y, inter.z);
            printf("\n");
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
