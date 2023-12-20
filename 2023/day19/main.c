#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 800
#define LEN_LINE 80

const size_t NONE = 1 << 30;

bool print = false;

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    char kind;
    size_t value;
    bool less;
    char next[20];
} criteria_t;

typedef struct {
    char name[20];
    criteria_t criteria[20];
    size_t criteria_count;
} elem_t;

size_t reduce_criteria(elem_t* elem) {
    size_t ret = 0;
    for (size_t i = 0; i < elem->criteria_count; i++) {
        ret += elem->criteria[i].value;
    }

    return ret;
}

size_t find_rule(elem_t* rules, size_t rule_count, char* name) {
    for (size_t i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].name, name) == 0) {
            return i;
        }
    }

    return NONE;
}

size_t find_match(elem_t* item, elem_t* rule) {
    for (size_t i = 0; i < rule->criteria_count; i++) {
        criteria_t* crit_rule = &rule->criteria[i];
        if (crit_rule->kind == '\0') {
            return i;
        }

        for (size_t j = 0; j < item->criteria_count; j++) {
            criteria_t* crit_item = &item->criteria[j];
            if (crit_rule->kind != crit_item->kind) {
                continue;
            }

            if (crit_rule->less && crit_item->value < crit_rule->value) {
                return i;
            } else if (!crit_rule->less && crit_item->value > crit_rule->value) {
                return i;
            }
        }
    }

    return NONE;
}

char* find_next(elem_t* item, elem_t* rule) {
    size_t index = find_match(item, rule);

    if (index == NONE) {
        return NULL;
    }

    return rule->criteria[index].next;
}

bool is_accepted(elem_t* item, elem_t* rules, size_t rule_count, size_t begin) {
    elem_t* rule = &rules[begin];

    char* next = NULL;
    while (*(next = find_next(item, rule)) != 'A' && *next != 'R') {
        rule = &rules[find_rule(rules, rule_count, next)];
        next = find_next(item, rule);
    }

    return *next == 'A';
}

size_t part_one(string_t* lines, size_t len_lines) {
    elem_t rules[LEN_LINES] = { 0, };
    size_t rule_count = 0;

    elem_t items[LEN_LINES] = { 0, };
    size_t item_count = 0;

    bool parse_elem = true;

    for (size_t i = 0; i < len_lines; i++) {
        if (lines[i].len == 0) {
            parse_elem = false;
        } else if (parse_elem) {
            char* buf = lines[i].str;

            elem_t* curr = &rules[rule_count++];
            lines[i].str[--lines[i].len] = '\0';

            char* token = strtok(buf, "{");
            buf += strlen(token) + 1;
            strcpy(curr->name, token);

            while ((token = strtok(buf, ",")) != NULL) {
                criteria_t* curr_crit = &curr->criteria[curr->criteria_count++];

                char* token_buf = buf;
                buf += strlen(token) + 1;

                if (strstr(token_buf, ":") != NULL) {
                    token_buf = strtok(token_buf, ":");

                    curr_crit->kind = token_buf[0];
                    curr_crit->less = token_buf[1] == '<' ? true : false;
                    token_buf += 2;
                    curr_crit->value = strtoll(token_buf, NULL, 10);

                    token_buf += strlen(token_buf) + 1;
                }

                strcpy(curr_crit->next, token_buf);
            }
        } else {
            lines[i].str++;
            lines[i].len--;
            lines[i].str[--lines[i].len] = '\0';

            elem_t* curr = &items[item_count++];

            char* token = strtok(lines[i].str, "=");
            curr->criteria[curr->criteria_count].kind = *token;
            token = strtok(NULL, ",");
            curr->criteria[curr->criteria_count].value = strtoll(token, NULL, 10);
            curr->criteria_count++;

            token = strtok(NULL, "=");
            curr->criteria[curr->criteria_count].kind = *token;
            token = strtok(NULL, ",");
            curr->criteria[curr->criteria_count].value = strtoll(token, NULL, 10);
            curr->criteria_count++;

            token = strtok(NULL, "=");
            curr->criteria[curr->criteria_count].kind = *token;
            token = strtok(NULL, ",");
            curr->criteria[curr->criteria_count].value = strtoll(token, NULL, 10);
            curr->criteria_count++;

            token = strtok(NULL, "=");
            curr->criteria[curr->criteria_count].kind = *token;
            token = strtok(NULL, ",");
            curr->criteria[curr->criteria_count].value = strtoll(token, NULL, 10);
            curr->criteria_count++;
        }
    }

    if (print) {
        for (size_t i = 0; i < rule_count; i++) {
            elem_t* curr = &rules[i];

            printf("%s{", curr->name);

            for (size_t j = 0; j < curr->criteria_count; j++) {
                criteria_t* curr_crit = &curr->criteria[j];

                if (curr_crit->kind != '\0') {
                    printf("%c%c%zu:", curr_crit->kind, curr_crit->less ? '<' : '>', curr_crit->value);
                }
                printf("%s%s", curr_crit->next, j < curr->criteria_count - 1 ? "," : "");
            }
            printf("}\n");
        }

        printf("\n");
    }

    size_t begin_index = find_rule(rules, rule_count, "in");
    size_t answer = 0;

    for (size_t i = 0; i < item_count; i++) {
        elem_t* curr = &items[i];

        if (print) {
            printf("{");

            for (size_t j = 0; j < curr->criteria_count; j++) {
                criteria_t* curr_crit = &curr->criteria[j];

                printf("%c=%zu%s", curr_crit->kind, curr_crit->value, j < curr->criteria_count - 1 ? "," : "");
            }
        }

        bool passed = is_accepted(curr, rules, rule_count, begin_index);
        size_t reduced = reduce_criteria(curr);

        answer += passed * reduced;

        if (print) {
            printf("} [%d, %zu]\n", passed, reduced);
        }
    }

    if (print) {
        printf("\n");
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    for (size_t i = 0; i < len_lines; i++) {
        printf("%4zu: \"%s\" (%zu)\n", i, lines[i].str, lines[i].len);
    }

    return 0;
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
