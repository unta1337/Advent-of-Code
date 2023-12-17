#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 4000
#define LEN_LINE 2000

#define ANSWER_TYPE int
#define ANSWER_FORMAT "%d"

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

ANSWER_TYPE part_one(string_t* lines, size_t len_lines) {
    int acc = 1;
    size_t cycle = 1;
    size_t signal_strength = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* op = strtok(lines[i].str, " ");

        if (strcmp(op, "noop") == 0) {
            switch (cycle) {
            case 20:
            case 60:
            case 100:
            case 140:
            case 180:
            case 220:
                signal_strength += cycle * acc;
                break;
            }

            cycle += 1;
        } else if (strcmp(op, "addx") == 0) {
            switch (cycle) {
            case 20:
            case 60:
            case 100:
            case 140:
            case 180:
            case 220:
                signal_strength += cycle * acc;
                break;
            }

            char* operand = strtok(NULL, " ");
            int value = strtol(operand, NULL, 0);
            cycle += 1;

            switch (cycle) {
            case 20:
            case 60:
            case 100:
            case 140:
            case 180:
            case 220:
                signal_strength += cycle * acc;
                break;
            }

            acc += value;
            cycle += 1;
        }
    }

    return signal_strength;
}

ANSWER_TYPE part_two(string_t* lines, size_t len_lines) {
    int acc = 1;
    size_t cycle = 1;
    size_t signal_strength = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* op = strtok(lines[i].str, " ");

        if (strcmp(op, "noop") == 0) {
            if (acc - (acc != 0) <= ((cycle - 1) % 40) && ((cycle - 1) % 40) <= acc + 1) {
                printf("#");
            } else {
                printf(".");
            }
            if ((cycle - 1) % 40 == 39) {
                printf("\n");
            }

            cycle += 1;
        } else if (strcmp(op, "addx") == 0) {
            if (acc - (acc != 0) <= ((cycle - 1) % 40) && ((cycle - 1) % 40) <= acc + 1) {
                printf("#");
            } else {
                printf(".");
            }
            if ((cycle - 1) % 40 == 39) {
                printf("\n");
            }

            char* operand = strtok(NULL, " ");
            int value = strtol(operand, NULL, 0);
            cycle += 1;

            if (acc - (acc != 0) <= ((cycle - 1) % 40) && ((cycle - 1) % 40) <= acc + 1) {
                printf("#");
            } else {
                printf(".");
            }
            if ((cycle - 1) % 40 == 39) {
                printf("\n");
            }

            acc += value;
            cycle += 1;
        }
    }

    return signal_strength;
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
