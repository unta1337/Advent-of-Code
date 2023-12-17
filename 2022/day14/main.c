#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define LEN_LINES 500
#define LEN_LINE 500

#define LEN_GRID 2000

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef struct {
    size_t row;
    size_t col;
} pos_t;

typedef struct {
    pos_t points[LEN_LINE];
    size_t point_count;
} border_t;

size_t part_one(string_t* lines, size_t len_lines) {
    pos_t sand_origin = { .row = 0, .col = 500 };

    border_t borders[LEN_LINES] = { 0, };
    size_t border_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* buf = lines[i].str;

        border_t* border = &borders[border_count];
        size_t* point_count = &border->point_count;

        char* token = NULL;
        while ((token = strtok(buf, " ")) != NULL) {
            char* token_buf = buf;
            buf += strlen(token) + 1;

            char* col_str = strtok(token_buf, ",");
            token_buf += strlen(col_str) + 1;
            border->points[*point_count].col = strtoll(col_str, NULL, 10);

            char* row_str = strtok(token_buf, " ");
            token_buf += strlen(row_str) + 1;
            border->points[*point_count].row = strtoll(row_str, NULL, 10);

            *point_count += 1;

            token = strtok(buf, " ");
            if (token != NULL) {
                buf += strlen(token) + 1;
            }
        }

        border_count++;
    }

    char** grid = (char**)malloc(LEN_GRID * sizeof(char*));
    for (size_t i = 0; i < LEN_GRID; i++) {
        grid[i] = (char*)calloc(LEN_GRID, sizeof(char));
    }

    pos_t top_left = { .row = 1LLU << 32, .col = 1LLU << 32 };
    pos_t bottom_right = { 0, };

    top_left.row = MIN(top_left.row, sand_origin.row);
    top_left.col = MIN(top_left.col, sand_origin.col);

    bottom_right.row = MAX(bottom_right.row, sand_origin.row);
    bottom_right.col = MAX(bottom_right.col, sand_origin.col);

    for (size_t i = 0; i < border_count; i++) {
        border_t* border = &borders[i];
        size_t point_count = border->point_count;

        for (size_t j = 0; j < point_count; j++) {
            top_left.row = MIN(top_left.row, border->points[j].row);
            top_left.col = MIN(top_left.col, border->points[j].col);

            bottom_right.row = MAX(bottom_right.row, border->points[j].row);
            bottom_right.col = MAX(bottom_right.col, border->points[j].col);

            if (j > 0) {
                size_t row_begin = MIN(border->points[j - 1].row, border->points[j].row);
                size_t row_end = MAX(border->points[j - 1].row, border->points[j].row);

                size_t col_begin = MIN(border->points[j - 1].col, border->points[j].col);
                size_t col_end = MAX(border->points[j - 1].col, border->points[j].col);

                for (size_t y = row_begin; y <= row_end; y++) {
                    for (size_t x = col_begin; x <= col_end; x++) {
                        grid[y][x] = '#';
                    }
                }
            }
        }
    }

    for (size_t i = 0; ; i++) {
        pos_t p = { 0, };
        pos_t s = sand_origin;

        while (p.row != s.row || p.col != s.col) {
            p = s;

            if (s.row == LEN_GRID - 10) {
                return i;
            }

            pos_t below = { .row = s.row + 1, .col = s.col };
            pos_t left = { .row = s.row + 1, .col = s.col - 1 };
            pos_t right = { .row = s.row + 1, .col = s.col + 1 };

            if (grid[below.row][below.col] == '\0') {
                s = below;
            } else if (grid[left.row][left.col] == '\0') {
                s = left;
            } else if (grid[right.row][right.col] == '\0') {
                s = right;
            } else {
                break;
            }
        }

        grid[s.row][s.col] = 'O';
    }

    return 0;
}

size_t part_two(string_t* lines, size_t len_lines) {
    pos_t sand_origin = { .row = 0, .col = 500 };

    border_t borders[LEN_LINES] = { 0, };
    size_t border_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* buf = lines[i].str;

        border_t* border = &borders[border_count];
        size_t* point_count = &border->point_count;

        char* token = NULL;
        while ((token = strtok(buf, " ")) != NULL) {
            char* token_buf = buf;
            buf += strlen(token) + 1;

            char* col_str = strtok(token_buf, ",");
            token_buf += strlen(col_str) + 1;
            border->points[*point_count].col = strtoll(col_str, NULL, 10);

            char* row_str = strtok(token_buf, " ");
            token_buf += strlen(row_str) + 1;
            border->points[*point_count].row = strtoll(row_str, NULL, 10);

            *point_count += 1;

            token = strtok(buf, " ");
            if (token != NULL) {
                buf += strlen(token) + 1;
            }
        }

        border_count++;
    }

    char** grid = (char**)malloc(LEN_GRID * sizeof(char*));
    for (size_t i = 0; i < LEN_GRID; i++) {
        grid[i] = (char*)calloc(LEN_GRID, sizeof(char));
    }

    pos_t top_left = { .row = 1LLU << 32, .col = 1LLU << 32 };
    pos_t bottom_right = { 0, };

    top_left.row = MIN(top_left.row, sand_origin.row);
    top_left.col = MIN(top_left.col, sand_origin.col);

    bottom_right.row = MAX(bottom_right.row, sand_origin.row);
    bottom_right.col = MAX(bottom_right.col, sand_origin.col);

    for (size_t i = 0; i < border_count; i++) {
        border_t* border = &borders[i];
        size_t point_count = border->point_count;

        for (size_t j = 0; j < point_count; j++) {
            top_left.row = MIN(top_left.row, border->points[j].row);
            top_left.col = MIN(top_left.col, border->points[j].col);

            bottom_right.row = MAX(bottom_right.row, border->points[j].row);
            bottom_right.col = MAX(bottom_right.col, border->points[j].col);

            if (j > 0) {
                size_t row_begin = MIN(border->points[j - 1].row, border->points[j].row);
                size_t row_end = MAX(border->points[j - 1].row, border->points[j].row);

                size_t col_begin = MIN(border->points[j - 1].col, border->points[j].col);
                size_t col_end = MAX(border->points[j - 1].col, border->points[j].col);

                for (size_t y = row_begin; y <= row_end; y++) {
                    for (size_t x = col_begin; x <= col_end; x++) {
                        grid[y][x] = '#';
                    }
                }
            }
        }
    }

    for (size_t i = 0; ; i++) {
        pos_t p = { 0, };
        pos_t s = sand_origin;

        size_t move_count = 0;
        while (p.row != s.row || p.col != s.col) {
            p = s;
            move_count++;

            if (s.row == bottom_right.row + 1) {
                break;
            }

            pos_t below = { .row = s.row + 1, .col = s.col };
            pos_t left = { .row = s.row + 1, .col = s.col - 1 };
            pos_t right = { .row = s.row + 1, .col = s.col + 1 };

            if (grid[below.row][below.col] == '\0') {
                s = below;
            } else if (grid[left.row][left.col] == '\0') {
                s = left;
            } else if (grid[right.row][right.col] == '\0') {
                s = right;
            } else {
                break;
            }
        }

        if (move_count == 1) {
            return i + 1;
        }

        grid[s.row][s.col] = 'O';
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
