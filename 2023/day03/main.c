#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define BUF_LEN 200

#define ANSWER_TYPE int
#define ANSWER_FORMAT "%d"

typedef struct {
    ANSWER_TYPE value;
    size_t row;
    size_t col_begin;
    size_t col_end;
} part_number_t;

typedef struct {
    size_t row;
    size_t col;
} pos_t;

ANSWER_TYPE parse_number(char* line, size_t line_len, size_t* col) {
    ANSWER_TYPE ret = 0;
    while (*col < line_len && isdigit(line[*col])) {
        ret *= 10;
        ret += line[*col] - '0';
        *col += 1;
    }

    return ret;
}

ANSWER_TYPE part_one(size_t line_number, char* line, size_t line_len) {
    static part_number_t numbers[4000] = { 0, };
    static size_t number_count = 0;
    static char prev_line[BUF_LEN] = "";

    if (line_number == 0) {
        memset(numbers, 0, 4000 * sizeof(part_number_t));
        number_count = 0;
        memset(prev_line, 0, BUF_LEN * sizeof(char));
    }

    int ret = 0;
    size_t col = 0;

    while (col < line_len) {
        if (isdigit(line[col])) {
            numbers[number_count].row = line_number;
            numbers[number_count].col_begin = col;
            numbers[number_count].value = parse_number(line, line_len, &col);
            numbers[number_count].col_end = col - 1;
            number_count++;

            if ((numbers[number_count - 1].col_begin - 1 < line_len
                && line[numbers[number_count - 1].col_begin - 1] != '.')
                || (numbers[number_count - 1].col_end + 1 < line_len
                && line[numbers[number_count - 1].col_end + 1] != '.')) {
                ret += numbers[number_count - 1].value;
                numbers[number_count - 1].value = 0;
            }

            for (size_t i = numbers[number_count - 1].col_begin - (numbers[number_count - 1].col_begin != 0)
                 ; i <= numbers[number_count - 1].col_end + 1; i++) {
                if (prev_line[i] != '.' && prev_line[i] != '\0' && !isdigit(prev_line[i])) {
                    ret += numbers[number_count - 1].value;
                    numbers[number_count - 1].value = 0;
                    break;
                }
            }
        } else {
            if (line[col] != '.') {
                for (size_t i = number_count - 1; i < number_count; i--) {
                    if (numbers[i].row == line_number) {
                        continue;
                    } else if (numbers[i].row < line_number - 1) {
                        break;
                    } else {
                        if (numbers[i].col_begin - (numbers[i].col_begin != 0) <= col
                            && col <= numbers[i].col_end + 1) {
                            ret += numbers[i].value;
                            numbers[i].value = 0;
                        }
                    }
                }
            }
            col++;
        }
    }

    memcpy(prev_line, line, line_len * sizeof(char));

    return ret;
}

ANSWER_TYPE part_two(size_t line_number, char* line, size_t line_len) {
    static part_number_t numbers[4000] = { 0, };
    static size_t number_count = 0;

    static pos_t gears[4000] = { 0, };
    static size_t gear_count = 0;

    if (line_number == 0) {
        memset(numbers, 0, 4000 * sizeof(part_number_t));
        number_count = 0;

        memset(gears, 0, 4000 * sizeof(pos_t));
        gear_count = 0;
    }

    size_t col = 0;

    while (col < line_len) {
        if (isdigit(line[col])) {
            numbers[number_count].row = line_number;
            numbers[number_count].col_begin = col;
            numbers[number_count].value = parse_number(line, line_len, &col);
            numbers[number_count].col_end = col - 1;
            number_count++;
        } else {
            if (line[col] == '*') {
                gears[gear_count].row = line_number;
                gears[gear_count].col = col;
                gear_count++;
            }
            col++;
        }
    }

    ANSWER_TYPE ret = 0;
    for (size_t i = 0; i < gear_count; i++) {
        ANSWER_TYPE ns[4000] = { 0, };
        size_t n_count = 0;

        for (size_t j = 0; j < number_count; j++) {
            if (gears[i].row - 1 <= numbers[j].row && numbers[j].row <= gears[i].row + 1) {
                if (numbers[j].col_begin - (numbers[j].col_begin != 0) <= gears[i].col
                    && gears[i].col <= numbers[j].col_end + 1) {
                    ns[n_count++] = numbers[j].value;
                }
            }
        }

        if (n_count == 2) {
            ret += ns[0] * ns[1];
        }
    }

    return ret;
}

void solve(const char* input_path) {
    // read input file.
    FILE* fp = fopen(input_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open input file \"%s\": %s.\n", input_path, strerror(errno));
        exit(errno);
    }

    // load input file into buffer line-by-line.
    char buffer[BUF_LEN] = "";
    size_t line_count = 0;

    ANSWER_TYPE answer = 0;

    for (size_t line_count = 1; fgets(buffer, BUF_LEN, fp) != NULL; line_count++) {
        // remove line feed character.
        size_t len = strlen(buffer);
        buffer[--len] = '\0';

#ifndef PART_TWO
        ANSWER_TYPE answer_line = part_one(line_count, buffer, len);
        answer += answer_line;
#else
        ANSWER_TYPE answer_line = part_two(line_count, buffer, len);
        answer = answer_line;
#endif
    }

    printf(ANSWER_FORMAT "\n", answer);

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
