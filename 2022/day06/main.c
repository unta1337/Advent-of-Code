#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 100
#define LEN_LINE 5000

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

size_t part_one(string_t* lines, size_t len_lines) {
    for (size_t i = 0; i < len_lines; i++) {
        size_t lookup[26] = { 0, };

        for (size_t j = 0; j < lines[i].len; j++) {
            lookup[lines[i].str[j] - 'a']++;
            if (j >= 4) {
                lookup[lines[i].str[j - 4] - 'a']--;

                bool ret = true;
                for (char c = 'a'; c <= 'z'; c++) {
                    if (lookup[c - 'a'] > 1) {
                        ret = false;
                        break;
                    }
                }

                if (ret) {
                    printf("%zu\n", j + 1);
                    break;
                }
            }
        }
    }

    return 0;
}

size_t part_two(string_t* lines, size_t len_lines) {
    for (size_t i = 0; i < len_lines; i++) {
        size_t lookup[26] = { 0, };

        for (size_t j = 0; j < lines[i].len; j++) {
            lookup[lines[i].str[j] - 'a']++;
            if (j >= 14) {
                lookup[lines[i].str[j - 14] - 'a']--;

                bool ret = true;
                for (char c = 'a'; c <= 'z'; c++) {
                    if (lookup[c - 'a'] > 1) {
                        ret = false;
                        break;
                    }
                }

                if (ret) {
                    printf("%zu\n", j + 1);
                    break;
                }
            }
        }
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
