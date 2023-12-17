#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 60
#define LEN_LINE 200

#define MAX_MINUTE 30
#define MAX_MINUTE2 26
#define LEN_IS_OPEN ((1 << 15) + 1)

const size_t NONE = 1 << 31;

size_t begin = 0;

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    char* name;
    size_t flow;

    char* next[LEN_LINES];
    size_t next_index[LEN_LINES];
    size_t next_count;
} valve_t;

size_t find_valve(valve_t* valves, size_t count, char* name) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(valves[i].name, name) == 0) {
            return i;
        }
    }

    return NONE;
}

size_t dfs(valve_t* valves, size_t count, size_t curr_index, size_t minute_left, unsigned int is_open, size_t* dp, size_t lookup_index[LEN_LINES]) {
    if (minute_left == 0) {
        return 0;
    }

    size_t lookuped_index = lookup_index[curr_index];
    size_t* ddp = &dp[curr_index * (MAX_MINUTE + 1) * LEN_IS_OPEN + minute_left * LEN_IS_OPEN + is_open];
    if (*ddp != NONE) {
        return *ddp;
    }

    valve_t* curr = &valves[curr_index];

    size_t ret = 0;
    for (size_t i = 0; i < curr->next_count; i++) {
        size_t next_index = curr->next_index[i];
        valve_t* next = &valves[next_index];

        size_t temp = dfs(valves, count, next_index, minute_left - 1, is_open, dp, lookup_index);
        ret = MAX(ret, temp);
    }

    if (curr->flow > 0 && !(is_open & (1 << lookuped_index))) {
        is_open ^= 1 << lookuped_index;
        size_t temp = (minute_left - 1) * curr->flow + dfs(valves, count, curr_index, minute_left - 1, is_open, dp, lookup_index);
        ret = MAX(ret, temp);
        is_open ^= 1 << lookuped_index;
    }

    return *ddp = ret;
}

size_t dfs2(valve_t* valves, size_t count, size_t curr_index, size_t minute_left, unsigned int is_open, size_t* dp, size_t lookup_index[LEN_LINES], bool flag) {
    if (minute_left == 0) {
        if (flag) {
            return dfs2(valves, count, begin, MAX_MINUTE2, is_open, dp, lookup_index, false);
        }

        return 0;
    }

    size_t lookuped_index = lookup_index[curr_index];
    size_t* ddp = &dp[curr_index * (MAX_MINUTE2 + 1) * LEN_IS_OPEN * 2 + minute_left * LEN_IS_OPEN * 2 + is_open * 2 + flag];
    if (*ddp != NONE) {
        return *ddp;
    }

    valve_t* curr = &valves[curr_index];

    size_t ret = 0;
    for (size_t i = 0; i < curr->next_count; i++) {
        size_t next_index = curr->next_index[i];
        valve_t* next = &valves[next_index];

        size_t temp = dfs2(valves, count, next_index, minute_left - 1, is_open, dp, lookup_index, flag);
        ret = MAX(ret, temp);
    }

    if (curr->flow > 0 && !(is_open & (1 << lookuped_index))) {
        is_open ^= 1 << lookuped_index;
        size_t temp = (minute_left - 1) * curr->flow + dfs2(valves, count, curr_index, minute_left - 1, is_open, dp, lookup_index, flag);
        ret = MAX(ret, temp);
        is_open ^= 1 << lookuped_index;
    }

    return *ddp = ret;
}

size_t part_one(string_t* lines, size_t len_lines) {
    valve_t valves[LEN_LINES] = { 0, };
    size_t valve_count = 0;

    // parse.
    for (size_t i = 0; i < len_lines; i++) {
        valve_t* curr = &valves[valve_count++];
        char* buf = lines[i].str;

        char* token = strtok(buf, " ");
        buf += strlen(token) + 1;

        curr->name = strtok(buf, " ");
        buf += strlen(valves->name) + 1;

        token = strtok(buf, "=");
        buf += strlen(token) + 1;

        token = strtok(buf, ";");
        buf += strlen(token) + 1;
        curr->flow = strtoll(token, NULL, 10);

        buf += strlen(" tunnels lead to valve");
        buf += *buf == 's' ? 2 : 1;

        while ((token = strtok(buf, ",")) != NULL) {
            buf += strlen(token) + 2;
            curr->next[curr->next_count++] = token;
        }
    }

    for (size_t i = 0; i < valve_count; i++) {
        valve_t* curr = &valves[i];
        size_t curr_index = find_valve(valves, valve_count, curr->name);

        for (size_t j = 0; j < curr->next_count; j++) {
            size_t next_index = find_valve(valves, valve_count, curr->next[j]);

            curr->next_index[j] = next_index;
        }
    }

    // index lookup.
    size_t index_lookup[LEN_LINES] = { 0, };
    size_t lookup_index = 0;

    for (size_t i = 0; i < valve_count; i++) {
        if (valves[i].flow > 0) {
            index_lookup[i] = lookup_index++;
        } else {
            index_lookup[i] = NONE;
        }
    }

    // do your stuff.
    begin = find_valve(valves, valve_count, "AA");

    size_t* dp = (size_t*)malloc(LEN_LINES * (MAX_MINUTE + 1) * LEN_IS_OPEN * sizeof(size_t));
    for (size_t i = 0; i < LEN_LINES; i++) {
        for (size_t j = 0; j < MAX_MINUTE + 1; j++) {
            for (size_t k = 0; k < LEN_IS_OPEN; k++) {
                dp[i * (MAX_MINUTE + 1) * LEN_IS_OPEN + j * LEN_IS_OPEN + k] = NONE;
            }
        }
    }

    size_t answer = dfs(valves, len_lines, begin, MAX_MINUTE, 0, dp, index_lookup);

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    valve_t valves[LEN_LINES] = { 0, };
    size_t valve_count = 0;

    // parse.
    for (size_t i = 0; i < len_lines; i++) {
        valve_t* curr = &valves[valve_count++];
        char* buf = lines[i].str;

        char* token = strtok(buf, " ");
        buf += strlen(token) + 1;

        curr->name = strtok(buf, " ");
        buf += strlen(valves->name) + 1;

        token = strtok(buf, "=");
        buf += strlen(token) + 1;

        token = strtok(buf, ";");
        buf += strlen(token) + 1;
        curr->flow = strtoll(token, NULL, 10);

        buf += strlen(" tunnels lead to valve");
        buf += *buf == 's' ? 2 : 1;

        while ((token = strtok(buf, ",")) != NULL) {
            buf += strlen(token) + 2;
            curr->next[curr->next_count++] = token;
        }
    }

    for (size_t i = 0; i < valve_count; i++) {
        valve_t* curr = &valves[i];
        size_t curr_index = find_valve(valves, valve_count, curr->name);

        for (size_t j = 0; j < curr->next_count; j++) {
            size_t next_index = find_valve(valves, valve_count, curr->next[j]);

            curr->next_index[j] = next_index;
        }
    }

    // index lookup.
    size_t index_lookup[LEN_LINES] = { 0, };
    size_t lookup_index = 0;

    for (size_t i = 0; i < valve_count; i++) {
        if (valves[i].flow > 0) {
            index_lookup[i] = lookup_index++;
        } else {
            index_lookup[i] = NONE;
        }
    }

    // do your stuff.
    begin = find_valve(valves, valve_count, "AA");

    size_t* dp = (size_t*)malloc(LEN_LINES * (MAX_MINUTE2 + 1) * LEN_IS_OPEN * 2 * sizeof(size_t));
    for (size_t i = 0; i < LEN_LINES; i++) {
        for (size_t j = 0; j < MAX_MINUTE2 + 1; j++) {
            for (size_t k = 0; k < LEN_IS_OPEN; k++) {
                for (size_t l = 0; l < 2; l++) {
                    dp[i * (MAX_MINUTE2 + 1) * LEN_IS_OPEN * 2 + j * LEN_IS_OPEN * 2 + k * 2 + l] = NONE;
                }
            }
        }
    }

    size_t answer = dfs2(valves, len_lines, begin, MAX_MINUTE2, 0, dp, index_lookup, true);

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
