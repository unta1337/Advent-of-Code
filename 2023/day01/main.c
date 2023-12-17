#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define BUF_LEN 200

char* numbers[10] = {
    "zero",
    "one",
    "two",
    "three",
    "four",
    "five",
    "six",
    "seven",
    "eight",
    "nine"
};

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
    int answer = 0;

    for (size_t line_count = 1; fgets(buffer, BUF_LEN, fp) != NULL; line_count++) {
        // remove line feed character.
        size_t len = strlen(buffer);
        buffer[--len] = '\0';

        // do your thing.
        int temp = -1;
        int first = -1;
        int last = -1;

        for (size_t i = 0; i < len; i++) {
            if (isdigit(buffer[i])) {
                temp = buffer[i] - '0';
                if (first == -1) first = temp;
                continue;
            }

#ifdef PART_TWO
            for (int j = 0; j <= 9; j++) {
                size_t n = strlen(numbers[j]);
                if (i + n <= len && strncmp(buffer + i, numbers[j], n) == 0) {
                    temp = j;
                    if (first == -1) first = temp;
                    continue;
                }
            }
#endif
        }

        if (temp == -1) {
            first = last = 0;
        } else {
            last = temp;
        }

        answer += first * 10 + last;
    }

    printf("%d\n", answer);

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
