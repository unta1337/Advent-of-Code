#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 2000
#define LEN_LINE 2000

#define DP_LEN 200

const size_t NONE = (1LLU << 32) - 1;

typedef struct {
    char* str;
    size_t len;
} string_t;

bool is_valid(char* springs, size_t* broken, size_t broken_count) {
    size_t brk[LEN_LINE] = { 0, };
    size_t brk_count = 0;

    size_t temp = 0;
    for (size_t i = 0; springs[i] != '\0'; i++) {
        if (springs[i] == '#') {
            temp++;
        }

        if (i > 0 && springs[i] == '.' && springs[i - 1] == '#') {
            brk[brk_count++] = temp;
            temp = 0;
        }
    }
    if (temp > 0) {
        brk[brk_count++] = temp;
    }

    if (brk_count != broken_count) {
        return false;
    }
    for (size_t i = 0; i < brk_count; i++) {
        if (brk[i] != broken[i]) {
            return false;
        }
    }

    return true;
}

size_t part_one(string_t* lines, size_t len_lines) {
    size_t answer = 0;
    for (size_t i = 0; i < len_lines; i++) {
        char* springs = strtok(lines[i].str, " ");

        size_t unknown_index[LEN_LINE] = { 0, };
        size_t unknown_count = 0;

        for (size_t j = 0; springs[j] != '\0'; j++) {
            if (springs[j] == '?') {
                unknown_index[unknown_count++] = j;
            }
        }

        size_t broken[LEN_LINE] = { 0, };
        size_t broken_count = 0;

        char* token = NULL;
        while ((token = strtok(NULL, ",")) != NULL) {
            broken[broken_count++] = strtoll(token, NULL, 10);
        }

        size_t res = 0;
        for (size_t j = 0; j < (1 << unknown_count); j++) {
            char buffer[LEN_LINE] = "";
            strcpy(buffer, springs);

            for (size_t k = 0; k < unknown_count; k++) {
                if (j & (1 << k)) {
                    buffer[unknown_index[k]] = '#';
                } else {
                    buffer[unknown_index[k]] = '.';
                }
            }

            res += is_valid(buffer, broken, broken_count);
        }

        answer += res;
    }

    return answer;
}

size_t helper_dp[DP_LEN][DP_LEN] = { 0, };

size_t helper(char* springs, size_t spring_count, size_t spring_index,
              size_t* broken, size_t broken_count, size_t broken_index) {
    if (spring_index > spring_count) {
        spring_index = spring_count;
    }
    if (broken_index > broken_count) {
        broken_index = broken_count;
    }

    if (spring_index == 0 && broken_index == 0) {
        for (size_t i = 0; i < DP_LEN; i++) {
            for (size_t j = 0; j < DP_LEN; j++) {
                helper_dp[i][j] = NONE;
            }
        }
    }

    if (spring_index == spring_count) {
        return broken_index >= broken_count ? 1 : 0;
    }
    if (broken_index == broken_count) {
        for (size_t i = spring_index; i < spring_count; i++) {
            if (springs[i] == '#') {
                return 0;
            }
        }

        return 1;
    }

    size_t *dp = &helper_dp[spring_index][broken_index];
    if (*dp != NONE) {
        return *dp;
    }

    size_t ret = 0;

    if (springs[spring_index] == '.' || springs[spring_index] == '?') {
        ret += helper(springs, spring_count, spring_index + 1,
                      broken, broken_count, broken_index);
    }
    if (springs[spring_index] == '#' || springs[spring_index] == '?') {
        bool dot_include = false;
        for (size_t i = spring_index; i < spring_index + broken[broken_index]; i++) {
            if (springs[i] == '.') {
                dot_include = true;
                break;
            }
        }

        if (broken[broken_index] <= spring_count - spring_index
            && !dot_include
            && (broken[broken_index] == spring_count - spring_index || springs[spring_index + broken[broken_index]] != '#')) {
            ret += helper(springs, spring_count, spring_index + broken[broken_index] + 1,
                          broken, broken_count, broken_index + 1);
        }
    }

    return *dp = ret;
}

size_t part_two(string_t* lines, size_t len_lines) {
    size_t answer = 0;
    size_t factor = 5;

    for (size_t i = 0; i < len_lines; i++) {
        char* token = strtok(lines[i].str, " ");;

        char springs[LEN_LINE] = "";
        strcpy(springs, token);
        size_t spring_count = strlen(springs);
        springs[spring_count++] = '?';

        spring_count *= factor;
        for (size_t i = 0; i < spring_count; i++) {
            springs[i] = springs[i % (spring_count / factor)];
        }
        springs[spring_count--] = '\0';

        size_t unknown_index[LEN_LINE] = { 0, };
        size_t unknown_count = 0;

        for (size_t j = 0; springs[j] != '\0'; j++) {
            if (springs[j] == '?') {
                unknown_index[unknown_count++] = j;
            }
        }

        unknown_count *= factor;
        for (size_t i = 0; i < unknown_count; i++) {
            unknown_index[i] = unknown_index[i % (unknown_count / factor)] + (unknown_count / factor) * (i / factor);
        }

        size_t broken[LEN_LINE] = { 0, };
        size_t broken_count = 0;

        while ((token = strtok(NULL, ",")) != NULL) {
            broken[broken_count++] = strtoll(token, NULL, 10);
        }

        broken_count *= factor;
        for (size_t i = 0; i < broken_count; i++) {
            broken[i] = broken[i % (broken_count / factor)];
        }

        size_t res = helper(springs, spring_count, 0,
                            broken, broken_count, 0);
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
