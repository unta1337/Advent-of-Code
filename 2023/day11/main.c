#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 2000
#define LEN_LINE 2000

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    size_t row;
    size_t col;
} pos_t;

size_t pos_dist(pos_t p, pos_t q) {
    size_t r = MAX(p.row, q.row) - MIN(p.row, q.row);
    size_t c = MAX(p.col, q.col) - MIN(p.col, q.col);

    return r + c;
}

size_t pos_dist_exp(pos_t p, pos_t q, bool* e_row, bool* e_col, size_t factor) {
    size_t r_min = MIN(p.row, q.row);
    size_t r_max = MAX(p.row, q.row);

    size_t c_min = MIN(p.col, q.col);
    size_t c_max = MAX(p.col, q.col);

    size_t r = 0;
    size_t c = 0;

    for (size_t i = r_min; i <= r_max; i++) {
        r += e_row[i] ? factor : 1;
    }
    for (size_t i = c_min; i <= c_max; i++) {
        c += e_col[i] ? factor : 1;
    }

    return r + c - 2;
}

size_t part_one(string_t* lines, size_t len_lines) {
    bool should_expand_row[LEN_LINE] = { 0, };
    for (size_t r = 0; r < len_lines; r++) {
        bool res = true;
        for (size_t c = 0; c < lines[0].len; c++) {
            if (lines[r].str[c] == '#') {
                res = false;
                break;
            }
        }

        should_expand_row[r] = res;
    }

    bool should_expand_col[LEN_LINE] = { 0, };
    for (size_t c = 0; c < lines[0].len; c++) {
        bool res = true;
        for (size_t r = 0; r < len_lines; r++) {
            if (lines[r].str[c] == '#') {
                res = false;
                break;
            }
        }

        should_expand_col[c] = res;
    }

    string_t expanded[LEN_LINES] = { 0, };
    for (size_t i = 0; i < LEN_LINES; i++) {
        expanded[i].str = (char*)calloc(LEN_LINE, sizeof(char));
    }
    size_t exp_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            size_t* index = &expanded[exp_count].len;

            expanded[exp_count].str[(*index)++] = lines[i].str[j];
            if (should_expand_col[j]) {
                expanded[exp_count].str[(*index)++] = lines[i].str[j];
            }
        }
        exp_count++;

        if (should_expand_row[i]) {
            strcpy(expanded[exp_count].str, expanded[exp_count - 1].str);
            expanded[exp_count].len = expanded[exp_count - 1].len;
            exp_count++;
        }
    }

    pos_t stars[LEN_LINE] = { 0, };
    size_t star_count = 0;

    for (size_t i = 0; i < exp_count; i++) {
        for (size_t j = 0; j < expanded[i].len; j++) {
            if (expanded[i].str[j] == '#') {
                stars[star_count].row = i;
                stars[star_count].col = j;
                star_count++;
            }
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < star_count; i++) {
        for (size_t j = i + 1; j < star_count; j++) {
            answer += pos_dist(stars[i], stars[j]);
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    bool should_expand_row[LEN_LINE] = { 0, };
    for (size_t r = 0; r < len_lines; r++) {
        bool res = true;
        for (size_t c = 0; c < lines[0].len; c++) {
            if (lines[r].str[c] == '#') {
                res = false;
                break;
            }
        }

        should_expand_row[r] = res;
    }

    bool should_expand_col[LEN_LINE] = { 0, };
    for (size_t c = 0; c < lines[0].len; c++) {
        bool res = true;
        for (size_t r = 0; r < len_lines; r++) {
            if (lines[r].str[c] == '#') {
                res = false;
                break;
            }
        }

        should_expand_col[c] = res;
    }

    pos_t stars[LEN_LINE] = { 0, };
    size_t star_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            if (lines[i].str[j] == '#') {
                stars[star_count].row = i;
                stars[star_count].col = j;
                star_count++;
            }
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < star_count; i++) {
        for (size_t j = i + 1; j < star_count; j++) {
            answer += pos_dist_exp(stars[i], stars[j], should_expand_row, should_expand_col, 1000000);
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
