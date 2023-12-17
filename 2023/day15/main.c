#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define LEN_LINES 20
#define LEN_LINE 30000

#define LEN_ITEM 200

const size_t NONE = 1 << 31;

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    char name[20];
    size_t value;
} lens_t;

typedef struct {
    lens_t items[LEN_ITEM];
    size_t count;
} box_t;

size_t find_lens(box_t* box, lens_t* lens) {
    for (size_t i = 0; i < box->count; i++) {
        if (strcmp(box->items[i].name, lens->name) == 0) {
            return i;
        }
    }

    return NONE;
}

size_t hash(char* str) {
    size_t ret = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        ret += str[i];
        ret *= 17;
        ret %= 256;
    }

    return ret;
}

size_t part_one(string_t* lines, size_t len_lines) {
    size_t answer = 0;
    for (size_t i = 0; i < len_lines; i++) {
        char* token = strtok(lines[i].str, ",");;
        answer += hash(token);

        while ((token = strtok(NULL, ",")) != NULL) {
            answer += hash(token);
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    box_t boxes[256] = { 0, };

    for (size_t i = 0; i < len_lines; i++) {
        char* buf = lines[i].str;

        char* token = NULL;
        while ((token = strtok(buf, ",")) != NULL) {
            char* lens_str = buf;
            buf += strlen(token) + 1;

            lens_t lens = {
                .name = "",
                .value = 0,
            };
            char op = '\0';
            size_t name_index = 0;

            while (isalpha(*lens_str)) {
                lens.name[name_index++] = *lens_str;
                lens_str++;
            }

            op = *lens_str;
            lens_str++;

            if (op == '=') {
                lens.value = *lens_str - '0';
            }

            size_t index = hash(lens.name);
            size_t find_index = find_lens(&boxes[index], &lens);

            switch (op) {
            case '=':
                if (find_index != NONE) {
                    boxes[index].items[find_index].value = lens.value;
                } else {
                    strcpy(boxes[index].items[boxes[index].count].name, lens.name);
                    boxes[index].items[boxes[index].count].value = lens.value;
                    boxes[index].count++;
                }
                break;
            case '-':
                if (find_index != NONE) {
                    for (size_t j = find_index; j < boxes[index].count; j++) {
                        strcpy(boxes[index].items[j].name,
                               boxes[index].items[j + 1].name);
                        boxes[index].items[j].value = boxes[index].items[j + 1].value;
                    }
                    boxes[index].count--;
                }
                break;
            }
        }
    }

    size_t answer = 0;
    for (size_t i = 0; i < 256; i++) {
        size_t res = 0;
        for (size_t j = 0; j < boxes[i].count; j++) {
            res += (i + 1) * boxes[i].items[j].value * (j + 1);
        }

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
