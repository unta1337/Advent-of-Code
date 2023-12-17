#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define BUF_LEN 200

int lookup[128] = {
    ['r'] = 12,
    ['g'] = 13,
    ['b'] = 14
};

int part_one(char* line) {
    char* cursor = line;

    char* token = strtok(cursor, ":");
    cursor += strlen(token) + 1;

    char game_name[BUF_LEN] = "";
    int game_no = 0;
    sscanf(token, "%s %d", game_name, &game_no);

    while ((token = strtok(cursor, ";")) != NULL) {
        cursor += strlen(token) + 1;

        char game[BUF_LEN] = "";
        strcpy(game, token);
        char* game_cursor = game;

        while ((token = strtok(game_cursor, ",")) != NULL) {
            game_cursor += strlen(token) + 1;

            int count = 0;
            char color[BUF_LEN] = "";
            sscanf(token, "%d %s", &count, color);

            if (count > lookup[color[0]]) {
                return 0;
            }
        }
    }

    return game_no;
}

int part_two(char* line) {
    char* cursor = line;

    char* token = strtok(cursor, ":");
    cursor += strlen(token) + 1;

    char game_name[BUF_LEN] = "";
    int game_no = 0;
    sscanf(token, "%s %d", game_name, &game_no);

    int res[128] = {
        ['r'] = 0,
        ['g'] = 0,
        ['b'] = 0
    };

    while ((token = strtok(cursor, ";")) != NULL) {
        cursor += strlen(token) + 1;

        char game[BUF_LEN] = "";
        strcpy(game, token);
        char* game_cursor = game;

        while ((token = strtok(game_cursor, ",")) != NULL) {
            game_cursor += strlen(token) + 1;

            int count = 0;
            char color[BUF_LEN] = "";
            sscanf(token, "%d %s", &count, color);

            res[color[0]] = MAX(res[color[0]], count);
        }
    }

    return res['r'] * res['g'] * res['b'];
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

#ifndef PART_TWO
        int line_answer = part_one(buffer);
#else
        int line_answer = part_two(buffer);
#endif

        answer += line_answer;
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
