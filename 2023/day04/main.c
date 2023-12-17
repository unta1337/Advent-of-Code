#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define LEN_LINES 4000
#define LEN_LINE 2000

#define ANSWER_TYPE int
#define ANSWER_FORMAT "%d"

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

int parse_number(char** buf) {
    int ret = 0;
    while (**buf != '\0' && !isdigit(**buf)) {
        *buf += 1;
    }

    while (**buf != '\0' && isdigit(**buf)) {
        ret *= 10;
        ret += **buf - '0';
        *buf += 1;
    }

    return ret;
}

ANSWER_TYPE part_one(string_t* lines, size_t len_lines) {
    ANSWER_TYPE answer = 0;
    for (size_t i = 0; i < len_lines; i++) {
        ANSWER_TYPE res = 0;
        char* buf = lines[i].str;

        char* token = strtok(buf, ":");
        buf += strlen(token) + 1;

        char winning_str[LEN_LINE] = "";
        token = strtok(buf, "|");
        buf += strlen(token) + 1;
        strcpy(winning_str, token);

        char my_str[LEN_LINE] = "";
        token = strtok(buf, "|");
        buf += strlen(token) + 1;
        strcpy(my_str, token);

        int winning_numbers[LEN_LINE] = { 0 ,};
        size_t winning_index = 0;
        buf = winning_str;
        while (*buf != '\0') {
            int num = parse_number(&buf);
            if (num != 0) {
                winning_numbers[winning_index++] = num;
            }
        }

        int my_numbers[LEN_LINE] = { 0 ,};
        size_t my_index = 0;
        buf = my_str;
        while (*buf != '\0') {
            int num = parse_number(&buf);
            if (num != 0) {
                my_numbers[my_index++] = num;
            }
        }

        for (size_t i = 0; i < my_index; i++) {
            for (size_t j = 0; j < winning_index; j++) {
                if (winning_numbers[j] == my_numbers[i]) {
                    if (res == 0) {
                        res = 1;
                    } else {
                        res *= 2;
                    }
                    break;
                }
            }
        }

        answer += res;
    }

    return answer;
}

ANSWER_TYPE part_two(string_t* lines, size_t len_lines) {
    size_t winning_count[LEN_LINE] = { 0, };
    for (size_t i = 0; i < len_lines; i++) {
        ANSWER_TYPE res = 0;
        char* buf = lines[i].str;

        char* token = strtok(buf, ":");
        buf += strlen(token) + 1;

        char winning_str[LEN_LINE] = "";
        token = strtok(buf, "|");
        buf += strlen(token) + 1;
        strcpy(winning_str, token);

        char my_str[LEN_LINE] = "";
        token = strtok(buf, "|");
        buf += strlen(token) + 1;
        strcpy(my_str, token);

        int winning_numbers[LEN_LINE] = { 0 ,};
        size_t winning_index = 0;
        buf = winning_str;
        while (*buf != '\0') {
            int num = parse_number(&buf);
            if (num != 0) {
                winning_numbers[winning_index++] = num;
            }
        }

        int my_numbers[LEN_LINE] = { 0 ,};
        size_t my_index = 0;
        buf = my_str;
        while (*buf != '\0') {
            int num = parse_number(&buf);
            if (num != 0) {
                my_numbers[my_index++] = num;
            }
        }

        for (size_t i = 0; i < my_index; i++) {
            for (size_t j = 0; j < winning_index; j++) {
                if (winning_numbers[j] == my_numbers[i]) {
                    res++;
                    break;
                }
            }
        }

        winning_count[i] = res;
    }

    size_t copies[LEN_LINE] = { 0, };
    for (size_t i = 0; i < len_lines; i++) {
        copies[i] = 1;
    }

    for (size_t i = 0; i < len_lines; i++) {
        for (size_t j = 0; j < winning_count[i]; j++) {
            copies[i + j + 1] += copies[i];
        }
    }

    ANSWER_TYPE answer = 0;
    for (size_t i = 0; i < len_lines; i++) {
        answer += copies[i];
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
    size_t line_count = 0;

    // load input file into buffer line-by-line.
    for (line_count = 0; fgets(lines[line_count].str, LEN_LINE, fp) != NULL; line_count++) {
        // remove line feed character.
        lines[line_count].len = strlen(lines[line_count].str);
        lines[line_count].str[--lines[line_count].len] = '\0';
    }

    // print answer.
#ifndef PART_TWO
    printf(ANSWER_FORMAT "\n", part_one(lines, line_count));
#else
    printf(ANSWER_FORMAT "\n", part_two(lines, line_count));
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
