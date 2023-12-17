#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 1000
#define LEN_LINE 1000

#define LEN_MONKEY 10
#define LEN_ITEM 2000000

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    size_t* items;
    size_t item_begin;
    size_t item_count;

    struct {
        char op;
        size_t value;
    } op;

    struct {
        size_t value;
        size_t if_true;
        size_t if_false;
    } test;

    size_t inspect_count;
} monkey_t;

int comp_desc(const void* p, const void* q) {
    size_t i = ((monkey_t*)p)->inspect_count;
    size_t j = ((monkey_t*)q)->inspect_count;

    return -((i > j) - (i < j));
}

char* chop_left(char* buf) {
    while (*buf != '\0' && *buf == ' ') {
        buf++;
    }

    return buf;
}

size_t part_one(string_t* lines, size_t len_lines) {
    monkey_t monkeys[LEN_MONKEY] = { 0, };
    for (size_t i = 0; i < LEN_MONKEY; i++) {
        monkeys[i].items = calloc(LEN_ITEM, sizeof(size_t));
    }
    size_t monkey_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        if (lines[i].len == 0) {
            continue;
        }

        char* buf = chop_left(lines[i].str);

        char* token = strtok(buf, " ");
        buf += strlen(token) + 1;

        if (strcmp(token, "Monkey") == 0) {
            monkey_count++;
        } else if (strcmp(token, "Starting") == 0) {
            token = strtok(buf, ":");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            while ((token = strtok(buf, ",")) != NULL) {
                buf += strlen(token) + 1;
                buf = chop_left(buf);

                size_t item = strtoll(token, NULL, 10);

                size_t* item_count = &monkeys[monkey_count - 1].item_count;
                monkeys[monkey_count - 1].items[*item_count] = item;
                *item_count += 1;
            }
        } else if (strcmp(token, "Operation:") == 0) {
            token = strtok(buf, "d");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            char op = *buf;

            token = strtok(buf, " ");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            size_t value = 0;
            if (strcmp(buf, "old") != 0) {
                value = strtoll(buf, NULL, 10);
            }

            monkeys[monkey_count - 1].op.op = op;
            monkeys[monkey_count - 1].op.value = value;
        } else if (strcmp(token, "Test:") == 0) {
            token = strtok(buf, "y");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            size_t value = strtoll(buf, NULL, 10);
            monkeys[monkey_count - 1].test.value = value;
        } else if (strcmp(token, "If") == 0) {
            token = strtok(buf, " ");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            if (strcmp(token, "true:") == 0) {
                token = strtok(buf, "y");
                buf += strlen(token) + 1;
                buf = chop_left(buf);

                size_t value = strtoll(buf, NULL, 10);
                monkeys[monkey_count - 1].test.if_true = value;
            } else {
                token = strtok(buf, "y");
                buf += strlen(token) + 1;
                buf = chop_left(buf);

                size_t value = strtoll(buf, NULL, 10);
                monkeys[monkey_count - 1].test.if_false = value;
            }
        }
    }

    for (size_t r = 0; r < 20; r++) {
        for (size_t i = 0; i < monkey_count; i++) {
            monkey_t* monkey = &monkeys[i];

            size_t iter = monkey->item_count;
            for (size_t j = 0; j < iter; j++) {
                size_t old = monkey->items[monkey->item_begin];
                size_t new = old;

                switch (monkey->op.op) {
                case '+':
                    new += monkey->op.value != 0 ? monkey->op.value : new;
                    break;
                case '*':
                    new *= monkey->op.value != 0 ? monkey->op.value : new;
                    break;
                }
                new /= 3;

                monkey_t* m = new % monkey->test.value == 0
                              ? &monkeys[monkey->test.if_true]
                              : &monkeys[monkey->test.if_false];
                m->items[m->item_begin + m->item_count++] = new;

                monkey->item_begin++;
                monkey->item_count--;
                monkey->inspect_count++;
            }
        }
    }

    qsort(monkeys, monkey_count, sizeof(monkey_t), comp_desc);

    return monkeys[0].inspect_count * monkeys[1].inspect_count;
}

size_t part_two(string_t* lines, size_t len_lines) {
    monkey_t monkeys[LEN_MONKEY] = { 0, };
    for (size_t i = 0; i < LEN_MONKEY; i++) {
        monkeys[i].items = calloc(LEN_ITEM, sizeof(size_t));
    }
    size_t monkey_count = 0;

    size_t mod = 1;
    for (size_t i = 0; i < len_lines; i++) {
        if (lines[i].len == 0) {
            continue;
        }

        char* buf = chop_left(lines[i].str);

        char* token = strtok(buf, " ");
        buf += strlen(token) + 1;

        if (strcmp(token, "Monkey") == 0) {
            monkey_count++;
        } else if (strcmp(token, "Starting") == 0) {
            token = strtok(buf, ":");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            while ((token = strtok(buf, ",")) != NULL) {
                buf += strlen(token) + 1;
                buf = chop_left(buf);

                size_t item = strtoll(token, NULL, 10);

                size_t* item_count = &monkeys[monkey_count - 1].item_count;
                monkeys[monkey_count - 1].items[*item_count] = item;
                *item_count += 1;
            }
        } else if (strcmp(token, "Operation:") == 0) {
            token = strtok(buf, "d");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            char op = *buf;

            token = strtok(buf, " ");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            size_t value = 0;
            if (strcmp(buf, "old") != 0) {
                value = strtoll(buf, NULL, 10);
            }

            monkeys[monkey_count - 1].op.op = op;
            monkeys[monkey_count - 1].op.value = value;
        } else if (strcmp(token, "Test:") == 0) {
            token = strtok(buf, "y");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            size_t value = strtoll(buf, NULL, 10);
            monkeys[monkey_count - 1].test.value = value;
            mod *= value;
        } else if (strcmp(token, "If") == 0) {
            token = strtok(buf, " ");
            buf += strlen(token) + 1;
            buf = chop_left(buf);

            if (strcmp(token, "true:") == 0) {
                token = strtok(buf, "y");
                buf += strlen(token) + 1;
                buf = chop_left(buf);

                size_t value = strtoll(buf, NULL, 10);
                monkeys[monkey_count - 1].test.if_true = value;
            } else {
                token = strtok(buf, "y");
                buf += strlen(token) + 1;
                buf = chop_left(buf);

                size_t value = strtoll(buf, NULL, 10);
                monkeys[monkey_count - 1].test.if_false = value;
            }
        }
    }

    for (size_t r = 0; r < 10000; r++) {
        for (size_t i = 0; i < monkey_count; i++) {
            monkey_t* monkey = &monkeys[i];

            size_t iter = monkey->item_count;
            for (size_t j = 0; j < iter; j++) {
                size_t old = monkey->items[monkey->item_begin];
                size_t new = old;
                size_t temp = 0;

                switch (monkey->op.op) {
                case '+':
                    temp = monkey->op.value != 0 ? monkey->op.value : new;
                    new = (new % mod) + (temp % mod);
                    break;
                case '*':
                    temp = monkey->op.value != 0 ? monkey->op.value : new;
                    new = (new % mod) * (temp % mod);
                    break;
                }
                new %= mod;

                monkey_t* m = new % monkey->test.value == 0
                              ? &monkeys[monkey->test.if_true]
                              : &monkeys[monkey->test.if_false];
                m->items[m->item_begin + m->item_count++] = new;

                monkey->item_begin++;
                monkey->item_count--;
                monkey->inspect_count++;
            }
        }
    }

    qsort(monkeys, monkey_count, sizeof(monkey_t), comp_desc);

    return monkeys[0].inspect_count * monkeys[1].inspect_count;
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
        lines[i].str = calloc(LEN_LINE, sizeof(char));
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
