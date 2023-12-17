#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 4000
#define LEN_LINE 2000

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

struct node_t {
    char* name;
    char* left;
    char* right;
};
typedef struct node_t node_t;

node_t* node_find(node_t* nodes, size_t node_count, char* name) {
    for (size_t i = 0; i < node_count; i++) {
        if (strcmp(nodes[i].name, name) == 0) {
            return &nodes[i];
        }
    }

    return NULL;
}

size_t get_gcd(size_t p, size_t q) {
    if (q == 0) {
        return p;
    }

    return get_gcd(q, p % q);
}

size_t get_lcm(size_t p, size_t q) {
    return p / get_gcd(p, q) * q;
}

size_t part_one(string_t* lines, size_t len_lines) {
    char* instructions = lines[0].str;
    size_t ins_count = strlen(instructions);

    node_t nodes[LEN_LINES] = { 0, };
    size_t node_count = 0;

    for (size_t i = 2; i < len_lines; i++) {
        char* node_name = strtok(lines[i].str, " ");
        strtok(NULL, "(");

        char* left = strtok(NULL, ",");
        char* right = strtok(NULL, ")");
        right++;

        nodes[node_count].name = node_name;
        nodes[node_count].left = left;
        nodes[node_count].right = right;
        node_count++;
    }

    node_t* node = node_find(nodes, node_count, "AAA");

    size_t index = 0;
    while (strcmp(node->name, "ZZZ") != 0) {
        char ins = instructions[index++ % ins_count];

        switch (ins) {
        case 'L':
            node = node_find(nodes, node_count, node->left);
            break;
        case 'R':
            node = node_find(nodes, node_count, node->right);
            break;
        }
    }

    return index;
}

size_t part_two(string_t* lines, size_t len_lines) {
    char* instructions = lines[0].str;
    size_t ins_count = strlen(instructions);

    node_t nodes[LEN_LINES] = { 0, };
    size_t node_count = 0;

    for (size_t i = 2; i < len_lines; i++) {
        char* node_name = strtok(lines[i].str, " ");
        strtok(NULL, "(");

        char* left = strtok(NULL, ",");
        char* right = strtok(NULL, ")");
        right++;

        nodes[node_count].name = node_name;
        nodes[node_count].left = left;
        nodes[node_count].right = right;
        node_count++;
    }

    node_t* begins[LEN_LINES] = { 0, };
    size_t begin_count = 0;

    size_t results[LEN_LINES] = { 0, };
    size_t res_count = 0;

    for (size_t i = 0; i < node_count; i++) {
        if (nodes[i].name[2] == 'A') {
            begins[begin_count++] = &nodes[i];
        }
    }

    for (size_t i = 0; i < begin_count; i++) {
        size_t index = 0;
        while (begins[i]->name[2] != 'Z') {
            char ins = instructions[index++ % ins_count];

            switch (ins) {
            case 'L':
                begins[i] = node_find(nodes, node_count, begins[i]->left);
                break;
            case 'R':
                begins[i] = node_find(nodes, node_count, begins[i]->right);
                break;
            }
        }

        results[res_count++] = index;
    }

    size_t answer = 1;
    for (size_t i = 0; i < res_count; i++) {
        answer = get_lcm(answer, results[i]);
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
