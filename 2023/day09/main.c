#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 4000
#define LEN_LINE 2000

typedef struct {
    char str[LEN_LINE];
    int len;
} string_t;

void calc_diff(int* seq, int count) {
    for (int i = 0; i < count - 1; i++) {
        int diff = seq[i + 1] - seq[i];
        seq[i] = diff;
    }
}

void rev(int* seq, int count) {
    int lower = 0;
    int upper = count - 1;

    while (lower < upper) {
        int temp = seq[lower];
        seq[lower] = seq[upper];
        seq[upper] = temp;

        lower++;
        upper--;
    }
}

int part_one(string_t* lines, int len_lines) {
    int answer = 0;

    for (int i = 0; i < len_lines; i++) {
        int seq[LEN_LINE] = { 0, };
        int seq_count = 0;

        char* buf = lines[i].str;
        char* token = NULL;

        while ((token = strtok(buf, " ")) != NULL) {
            buf += strlen(token) + 1;
            seq[seq_count++] = strtoll(token, NULL, 10);
        }

        for (int c = seq_count; c > 0; c--) {
            calc_diff(seq, c);

            bool done = true;
            for (size_t j = 0; j < c; j++) {
                if (seq[j] != 0) {
                    done = false;
                    break;
                }
            }

            if (done) {
                int res = 0;
                for (int j = c; j < seq_count; j++) {
                    res += seq[j];
                }
                answer += res;

                break;
            }
        }
    }

    return answer;
}

int part_two(string_t* lines, int len_lines) {
    int answer = 0;

    for (int i = 0; i < len_lines; i++) {
        int seq[LEN_LINE] = { 0, };
        int seq_count = 0;

        char* buf = lines[i].str;
        char* token = NULL;

        while ((token = strtok(buf, " ")) != NULL) {
            buf += strlen(token) + 1;
            seq[seq_count++] = strtoll(token, NULL, 10);
        }

        rev(seq, seq_count);

        for (int c = seq_count; c > 0; c--) {
            calc_diff(seq, c);

            bool done = true;
            for (size_t j = 0; j < c; j++) {
                if (seq[j] != 0) {
                    done = false;
                    break;
                }
            }

            if (done) {
                int res = 0;
                for (int j = c; j < seq_count; j++) {
                    res += seq[j];
                }
                answer += res;

                break;
            }
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
    int line_count = 0;

    // load input file into buffer line-by-line.
    for (line_count = 0; fgets(lines[line_count].str, LEN_LINE, fp) != NULL; line_count++) {
        // remove line feed character.
        lines[line_count].len = strlen(lines[line_count].str);
        lines[line_count].str[--lines[line_count].len] = '\0';
    }

    // print answer.
#ifndef PART_TWO
    printf("%d\n", part_one(lines, line_count));
#else
    printf("%d\n", part_two(lines, line_count));
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
