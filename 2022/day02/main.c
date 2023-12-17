#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define BUF_LEN 200

int am_i_win(char me, char other) {
    switch (me) {
    case 'X': switch (other) {
                  case 'A': return 0;
                  case 'B': return -1;
                  case 'C': return 1;
              }
    case 'Y': switch (other) {
                  case 'A': return 1;
                  case 'B': return 0;
                  case 'C': return -1;
              }
    case 'Z': switch (other) {
                  case 'A': return -1;
                  case 'B': return 1;
                  case 'C': return 0;
              }
    }
}

char what_should_i_take(char me, char other) {
    switch (me) {
    case 'X': switch (other) {
                  case 'A': return 'Z';
                  case 'B': return 'X';
                  case 'C': return 'Y';
              }
    case 'Y': switch (other) {
                  case 'A': return 'X';
                  case 'B': return 'Y';
                  case 'C': return 'Z';
              }
    case 'Z': switch (other) {
                  case 'A': return 'Y';
                  case 'B': return 'Z';
                  case 'C': return 'X';
              }
    }
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

    int answer = 0;

    for (size_t line_count = 1; fgets(buffer, BUF_LEN, fp) != NULL; line_count++) {
        // remove line feed character.
        size_t len = strlen(buffer);
        buffer[--len] = '\0';

        // do your thing.
        char other = buffer[0];
        char me = buffer[len - 1];

#ifndef PART_TWO
        int score = me == 'X' ? 1 : (me == 'Y' ? 2 : 3);

        switch (am_i_win(me, other)) {
        case 0:
            score += 3;
            break;
        case 1:
            score += 6;
            break;
        }
#else
        int score = me == 'X' ? 0 : (me == 'Y' ? 3 : 6);

        switch (what_should_i_take(me, other)) {
        case 'X':
            score += 1;
            break;
        case 'Y':
            score += 2;
            break;
        case 'Z':
            score += 3;
            break;
        }
#endif

        answer += score;
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
