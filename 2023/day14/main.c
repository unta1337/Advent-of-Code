#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 200
#define LEN_LINE 200

#define LEN_CACHE 1000

const size_t NONE = 1 << 31;

typedef struct {
    char* str;
    size_t len;
} string_t;

bool comp_lines(string_t* p, string_t* q, size_t rows) {
    for (size_t i = 0; i < rows; i++) {
        if (strcmp(p[i].str, q[i].str) != 0) {
            return false;
        }
    }

    return true;
}

size_t find_string(string_t** haystack, size_t haystack_count, string_t* needle, size_t needle_count) {
    for (size_t i = 0; i < haystack_count; i++) {
        string_t* cmp = haystack[i];

        if (comp_lines(cmp, needle, needle_count)) {
            return i;
        }
    }

    return NONE;
}

void drop_rock(string_t* lines, size_t row, size_t col) {
    size_t r = row;
    while (r - 1 < lines->len && lines[r - 1].str[col] == '.') {
        r--;
    }

    if (r < lines->len && r != row) {
        lines[r].str[col] = 'O';
        lines[row].str[col] = '.';
    }
}

size_t count_rock(string_t* line) {
    size_t ret = 0;
    for (size_t i = 0; i < line->len; i++) {
        ret += line->str[i] == 'O';
    }

    return ret;
}

void rorate_cw(string_t* lines, size_t len_lines) {
    string_t rotated[LEN_LINES] = { 0, };
    for (size_t i = 0; i < len_lines; i++) {
        rotated[i].str = (char*)calloc(LEN_LINE, sizeof(char));
    }

    for (size_t i = 0; i < len_lines; i++) {
        rotated[i].len = lines[i].len;
        for (size_t j = 0; j < lines[i].len; j++) {
            rotated[j].str[i] = lines[i].str[j];
        }
    }

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < rotated[i].len / 2; j++) {
            char temp = rotated[i].str[j];
            rotated[i].str[j] = rotated[i].str[rotated[i].len - j - 1];
            rotated[i].str[rotated[i].len - j - 1] = temp;
        }
    }

    for (size_t i = 0; i < lines->len; i++) {
        strcpy(lines[i].str, rotated[i].str);
    }
}

size_t part_one(string_t* lines, size_t len_lines) {
    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < lines[i].len; j++) {
            if (lines[i].str[j] == 'O') {
                drop_rock(lines, i, j);
            }
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < len_lines; i++) {
        answer += count_rock(&lines[i]) * (len_lines - i);
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    string_t** cache = (string_t**)calloc(LEN_CACHE, sizeof(string_t*));
    size_t cache_count = 0;

    size_t* res_cache = (size_t*)calloc(LEN_CACHE, sizeof(size_t));
    size_t res_cache_count = 0;

    for (size_t i = 0; i < LEN_CACHE; i++) {
        cache[i] = (string_t*)calloc(LEN_LINES, sizeof(string_t));

        for (size_t j = 0; j < LEN_LINE; j++) {
            cache[i][j].str = (char*)calloc(LEN_LINE, sizeof(char));
        }
    }

    size_t cycles = 1000000000 - 1;
    size_t cycle_begin = 0;
    size_t prev = 0;

    for (size_t c = 0; c <= cycles; c++) {
        for (size_t d = 0; d < 4; d++) {
            for (size_t i = 0; i < len_lines; i++) {
                for (size_t j = 0; j < lines[i].len; j++) {
                    if (lines[i].str[j] == 'O') {
                        drop_rock(lines, i, j);
                    }
                }
            }
            rorate_cw(lines, len_lines);
        }

        size_t cache_index = find_string(cache, cache_count, lines, len_lines);
        if (cache_index != NONE) {
            cycle_begin = cache_index;
            break;
        } else {
            string_t* cmp = cache[cache_count];

            for (size_t i = 0; i < len_lines; i++) {
                strcpy(cmp[i].str, lines[i].str);
            }

            cache_count++;

            size_t res = 0;
            for (size_t i = 0; i < len_lines; i++) {
                res += count_rock(&lines[i]) * (len_lines - i);
            }

            res_cache[res_cache_count++] = res;
        }
    }

    return res_cache[cycle_begin + (cycles - cycle_begin) % (res_cache_count - cycle_begin)];
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
