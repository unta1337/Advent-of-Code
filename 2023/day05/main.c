#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define INF SIZE_MAX
#define LEN_LINES 1000
#define LEN_LINE 1000

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

typedef enum {
    SEED_TO_SOIL,
    SOIL_TO_FERTILIZER,
    FERTILIZER_TO_WATER,
    WATER_TO_LIGHT,
    LIGHT_TO_TEMPERATURE,
    TEMPERATURE_TO_HUMIDITY,
    HUMIDITY_TO_LOCATION,
    NUM_MAP_KIND
} map_kind_t;

char* map_kinds[NUM_MAP_KIND] = {
    "seed-to-soil",
    "soil-to-fertilizer",
    "fertilizer-to-water",
    "water-to-light",
    "light-to-temperature",
    "temperature-to-humidity",
    "humidity-to-location",
};

typedef struct {
    size_t dest_begin;
    size_t src_begin;
    size_t count;
} seg_t;

typedef struct {
    seg_t segs[LEN_LINE];
    size_t count;
} map_t;

typedef struct {
    map_t maps[NUM_MAP_KIND];
    size_t count;
} box_t;

size_t seg_map(seg_t* seg, size_t src) {
    if (seg->src_begin <= src && src < seg->src_begin + seg->count) {
        return seg->dest_begin + (src - seg->src_begin);
    }

    return src;
}

size_t map_map(map_t* map, size_t src) {
    for (size_t i = 0; i < map->count; i++) {
        size_t mapped = seg_map(&map->segs[i], src);
        if (mapped != src) {
            return mapped;
        }
    }

    return src;
}

size_t box_map(box_t* box, size_t src) {
    for (size_t i = 0; i < box->count; i++) {
        src = map_map(&box->maps[i], src);
    }

    return src;
}

size_t part_one(string_t* lines, size_t len_lines) {
    size_t seeds[LEN_LINE] = { 0, };
    size_t seed_count = 0;

    box_t box = {
        .maps = { 0, },
        .count = NUM_MAP_KIND
    };
    size_t current_map_index = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* token;

        if (lines[i].len == 0) {
            continue;
        } else if (strncmp(lines[i].str, "seeds", strlen("seeds")) == 0) {
            token = strtok(lines[i].str, ":");
            while (*token == ' ') {
                token++;
            }

            while ((token = strtok(NULL, " ")) != NULL) {
                size_t seed = 0;
                sscanf(token, "%zu", &seed);
                seeds[seed_count++] = seed;
            }
            continue;
        } else {
            bool find = false;
            for (size_t j = 0; j < NUM_MAP_KIND; j++) {
                if (strncmp(lines[i].str, map_kinds[j], strlen(map_kinds[j])) == 0) {
                    current_map_index = j;
                    find = true;
                }
            }

            if (find) {
                continue;
            }
        }

        size_t* seg_index = &box.maps[current_map_index].count;
        sscanf(lines[i].str, "%zu %zu %zu", &box.maps[current_map_index].segs[*seg_index].dest_begin
                                          , &box.maps[current_map_index].segs[*seg_index].src_begin
                                          , &box.maps[current_map_index].segs[*seg_index].count);
        *seg_index += 1;
    }

    size_t answer = INF;
    for (size_t i = 0; i < seed_count; i++) {
        answer = MIN(answer, box_map(&box, seeds[i]));
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    size_t seeds[LEN_LINE] = { 0, };
    size_t seed_count = 0;

    box_t box = {
        .maps = { 0, },
        .count = NUM_MAP_KIND
    };
    size_t current_map_index = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* token;

        if (lines[i].len == 0) {
            continue;
        } else if (strncmp(lines[i].str, "seeds", strlen("seeds")) == 0) {
            token = strtok(lines[i].str, ":");
            while (*token == ' ') {
                token++;
            }

            while ((token = strtok(NULL, " ")) != NULL) {
                size_t seed = 0;
                sscanf(token, "%zu", &seed);
                seeds[seed_count++] = seed;
            }
            continue;
        } else {
            bool find = false;
            for (size_t j = 0; j < NUM_MAP_KIND; j++) {
                if (strncmp(lines[i].str, map_kinds[j], strlen(map_kinds[j])) == 0) {
                    current_map_index = j;
                    find = true;
                }
            }

            if (find) {
                continue;
            }
        }

        size_t* seg_index = &box.maps[current_map_index].count;
        sscanf(lines[i].str, "%zu %zu %zu", &box.maps[current_map_index].segs[*seg_index].dest_begin
                                          , &box.maps[current_map_index].segs[*seg_index].src_begin
                                          , &box.maps[current_map_index].segs[*seg_index].count);
        *seg_index += 1;
    }

    size_t answer = INF;
    for (size_t i = 0; i < seed_count; i += 2) {
        printf("%zu/%zu\n", i / 2 + 1, seed_count / 2);

        for (size_t j = 0; j < seeds[i + 1]; j++) {
            answer = MIN(answer, box_map(&box, seeds[i] + j));
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
