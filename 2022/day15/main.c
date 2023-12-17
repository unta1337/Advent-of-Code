#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define ABS(x) ((x) > 0 ? (x) : -(x))

#define LEN_LINES 2000
#define LEN_LINE 2000

typedef struct {
    char* str;
    int len;
} string_t;

typedef struct {
    int row;
    int col;
} pos_t;

typedef struct {
    pos_t pos;
    pos_t beacon;
    int dist;
} sensor_t;

typedef struct {
    int begin;
    int end;
} range_t;

pos_t* find_pos(pos_t* pos_list, int count, pos_t* target) {
    for (int i = 0; i < count; i++) {
        if (pos_list[i].row == target->row && pos_list[i].col == target->col) {
            return &pos_list[i];
        }
    }

    return NULL;
}

int comp_range(const void* p, const void* q) {
    int i = ((range_t*)p)->begin;
    int j = ((range_t*)q)->begin;

    return (i > j) - (i < j);
}

int get_dist(pos_t* p, pos_t* q) {
    return ABS(p->row - q->row) + ABS(p->col - q->col);
}

int y = 0;

int row_limit = 0;
int col_limit = 0;

int part_one(string_t* lines, int len_lines) {
    sensor_t sensors[LEN_LINES] = { 0, };
    int sensor_count = 0;

    pos_t beacons[LEN_LINES] = { 0, };
    int beacon_count = 0;

    for (int i = 0; i < len_lines; i++) {
        strtok(lines[i].str, "=");

        char* sensor_col = strtok(NULL,  ",");
        strtok(NULL, "=");
        char* sensor_row = strtok(NULL,  ":");

        strtok(NULL, "=");
        char* beacon_col = strtok(NULL, ",");
        strtok(NULL, "=");
        char* beacon_row = strtok(NULL, " ");

        sensors[sensor_count].pos.row = strtoll(sensor_row, NULL, 10);
        sensors[sensor_count].pos.col = strtoll(sensor_col, NULL, 10);
        sensors[sensor_count].beacon.row = strtoll(beacon_row, NULL, 10);
        sensors[sensor_count].beacon.col = strtoll(beacon_col, NULL, 10);
        sensors[sensor_count].dist = get_dist(&sensors[sensor_count].pos,
                                              &sensors[sensor_count].beacon);

        if (find_pos(beacons, beacon_count, &sensors[sensor_count].beacon) == NULL) {
            beacons[beacon_count++] = sensors[sensor_count].beacon;
        }

        sensor_count++;
    }

    range_t ranges[LEN_LINES] = { 0, };
    int range_count = 0;

    for (int j = 0; j < sensor_count; j++) {
        sensor_t* sensor = &sensors[j];

        int R = sensor->dist;
        int dr = ABS(y - sensor->pos.row);
        int r = R - dr;

        if (r < 0) {
            continue;
        } else {
            ranges[range_count].begin = sensor->pos.col - r;
            ranges[range_count].end = sensor->pos.col + r;
            range_count++;
        }
    }

    qsort(ranges, range_count, sizeof(range_t), comp_range);

    range_t merged[LEN_LINES] = { 0, };
    int merged_count = 0;

    for (int i = 1; i < range_count; i++) {
        range_t* prev = &ranges[i - 1];
        range_t* curr = &ranges[i];

        if (curr->begin > prev->end) {
            merged[merged_count++] = *prev;
        } else if (curr->begin <= prev->end) {
            curr->begin = prev->begin;
            curr->end = MAX(prev->end, curr->end);
        }
    }
    merged[merged_count++] = ranges[range_count - 1];

    int total = 0;
    int valid = 0;

    for (int i = 0; i < merged_count; i++) {
        range_t* range = &merged[i];

        total += range->end - range->begin + 1;
        for (int j = 0; j < beacon_count; j++) {
            pos_t* beacon = &beacons[j];
            valid += beacon->row == y
                     && range->begin <= beacon->col
                     && beacon->col <= range->end;
        }
    }

    return total - valid;
}

int part_two(string_t* lines, int len_lines) {
    sensor_t sensors[LEN_LINES] = { 0, };
    int sensor_count = 0;

    pos_t beacons[LEN_LINES] = { 0, };
    int beacon_count = 0;

    for (int i = 0; i < len_lines; i++) {
        strtok(lines[i].str, "=");

        char* sensor_col = strtok(NULL,  ",");
        strtok(NULL, "=");
        char* sensor_row = strtok(NULL,  ":");

        strtok(NULL, "=");
        char* beacon_col = strtok(NULL, ",");
        strtok(NULL, "=");
        char* beacon_row = strtok(NULL, " ");

        sensors[sensor_count].pos.row = strtoll(sensor_row, NULL, 10);
        sensors[sensor_count].pos.col = strtoll(sensor_col, NULL, 10);
        sensors[sensor_count].beacon.row = strtoll(beacon_row, NULL, 10);
        sensors[sensor_count].beacon.col = strtoll(beacon_col, NULL, 10);
        sensors[sensor_count].dist = get_dist(&sensors[sensor_count].pos,
                                              &sensors[sensor_count].beacon);

        if (find_pos(beacons, beacon_count, &sensors[sensor_count].beacon) == NULL) {
            beacons[beacon_count++] = sensors[sensor_count].beacon;
        }

        sensor_count++;
    }

    for (int y = 0; y < row_limit; y++) {
        range_t ranges[LEN_LINES] = { 0, };
        int range_count = 0;

        for (int j = 0; j < sensor_count; j++) {
            sensor_t* sensor = &sensors[j];

            int R = sensor->dist;
            int dr = ABS(y - sensor->pos.row);
            int r = R - dr;

            if (r < 0) {
                continue;
            } else {
                ranges[range_count].begin = sensor->pos.col - r;
                ranges[range_count].end = sensor->pos.col + r;
                range_count++;
            }
        }

        qsort(ranges, range_count, sizeof(range_t), comp_range);

        range_t merged[LEN_LINES] = { 0, };
        int merged_count = 0;

        for (int i = 1; i < range_count; i++) {
            range_t* prev = &ranges[i - 1];
            range_t* curr = &ranges[i];

            if (curr->begin > prev->end) {
                merged[merged_count++] = *prev;
            } else if (curr->begin <= prev->end) {
                curr->begin = prev->begin;
                curr->end = MAX(prev->end, curr->end);
            }
        }
        merged[merged_count++] = ranges[range_count - 1];

        for (int i = 1; i < merged_count; i++) {
            range_t* prev = &merged[i - 1];
            range_t* curr = &merged[i];

            if (curr->begin - prev->end > 1) {
                printf("%lld\n", (prev->end + 1LL) * 4000000LL + y);
                return 0;
            }
        }
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
    for (int i = 0; i < LEN_LINES; i++) {
        lines[i].str = (char*)calloc(LEN_LINE, sizeof(char));
    }
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

    y = strstr(argv[1], "input") != NULL ? 2000000 : 10;
    row_limit = 2 * y;
    col_limit = 2 * y;

    solve(argv[1]);

    return 0;
}
