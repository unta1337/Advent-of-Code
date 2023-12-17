#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 2000
#define LEN_LINE 2000

typedef struct {
    char* str;
    size_t len;
} string_t;

bool comp_row(string_t* lines, size_t p, size_t q) {
    return strcmp(lines[p].str, lines[q].str) == 0;
}

bool comp_col(string_t* lines, size_t row_begin, size_t row_end, size_t p, size_t q) {
    for (size_t i = row_begin; i < row_end; i++) {
        if (lines[i].str[p] != lines[i].str[q]) {
            return false;
        }
    }

    return true;
}

size_t check_row(string_t* lines, size_t begin, size_t end, size_t row) {
    size_t upper = row;
    size_t lower = row + 1;

    size_t ret = 0;
    while (upper >= begin && upper < end && lower < end
           && comp_row(lines, upper, lower)) {
        ret++;
        upper--;
        lower++;
    }

    return ret;
}

size_t check_col(string_t* lines, size_t begin, size_t end, size_t col) {
    size_t left = col;
    size_t right = col + 1;

    size_t ret = 0;
    while (left < lines[begin].len && right < lines[begin].len
           && comp_col(lines, begin, end, left, right)) {
        ret++;
        left--;
        right++;
    }

    return ret;
}

size_t diff_row(string_t* lines, size_t p, size_t q) {
    size_t ret = 0;
    for (size_t i = 0; i < lines[p].len; i++) {
        ret += lines[p].str[i] != lines[q].str[i];
    }

    return ret;
}

size_t diff_col(string_t* lines, size_t row_begin, size_t row_end, size_t p, size_t q) {
    size_t ret = 0;
    for (size_t i = row_begin; i < row_end; i++) {
        ret += lines[i].str[p] != lines[i].str[q];
    }

    return ret;
}

size_t check_diff_row(string_t* lines, size_t begin, size_t end, size_t row) {
    size_t upper = row;
    size_t lower = row + 1;

    size_t ret = 0;
    size_t count = 0;

    while (upper >= begin && upper < end && lower < end
           && (count += diff_row(lines, upper, lower), count) < 2) {
        ret++;
        upper--;
        lower++;
    }

    return ret * (count == 1);
}

size_t check_diff_col(string_t* lines, size_t begin, size_t end, size_t col) {
    size_t left = col;
    size_t right = col + 1;

    size_t ret = 0;
    size_t count = 0;

    while (left < lines[begin].len && right < lines[begin].len
           && (count += diff_col(lines, begin, end, left, right), count) < 2) {
        ret++;
        left--;
        right++;
    }

    return ret * (count == 1);
}

size_t part_one(string_t* lines, size_t len_lines) {
    size_t begin = 0;
    size_t end = 0;
    size_t answer = 0;

    for (size_t i = 0; i < len_lines; i++) {
        if (lines[i].len == 0 || i == len_lines - 1) {
            end = i + (i == len_lines - 1);

            size_t res = 0;

            for (size_t j = begin; j < end - 1; j++) {
                size_t temp = check_row(lines, begin, end, j);
                if (temp == j - begin + 1 || temp == end - j - 1) {
                    res += (j - begin + 1) * 100;
                }
            }
            for (size_t j = 0; j < lines[begin].len - 1; j++) {
                size_t temp = check_col(lines, begin, end, j);
                if (temp == j + 1 || temp == lines[begin].len - j - 1) {
                    res += j + 1;
                }
            }

            answer += res;
            begin = i + 1;
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    size_t begin = 0;
    size_t end = 0;
    size_t answer = 0;

    for (size_t i = 0; i < len_lines; i++) {
        if (lines[i].len == 0 || i == len_lines - 1) {
            end = i + (i == len_lines - 1);

            size_t res = 0;

            for (size_t j = begin; j < end - 1; j++) {
                size_t temp = check_diff_row(lines, begin, end, j);
                if (temp == j - begin + 1 || temp == end - j - 1) {
                    res += (j - begin + 1) * 100;
                }
            }
            for (size_t j = 0; j < lines[begin].len - 1; j++) {
                size_t temp = check_diff_col(lines, begin, end, j);
                if (temp == j + 1 || temp == lines[begin].len - j - 1) {
                    res += j + 1;
                }
            }

            answer += res;
            begin = i + 1;
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
