#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 60
#define LEN_LINE 200

typedef struct {
    char* str;
    size_t len;
} string_t;

typedef enum {
    LOW,
    HIGH
} pulse_t;

typedef enum {
    BROADCASTER,
    FLIP_FLOP,
    CONJUNCTION
} module_type_t;

module_type_t module_type_lookup[128] = {
    ['%'] = FLIP_FLOP,
    ['&'] = CONJUNCTION
};

typedef struct {
    char* name;
    char* next[LEN_LINES];
    size_t next_count;
} broadercaster_t;

typedef struct {
    char* name;
    bool is_on;
    char* next[LEN_LINES];
    size_t next_count;
} flip_flop_t;

typedef struct {
    char* name;
    char* inputs[LEN_LINES];
    size_t input_count;
    pulse_t last_pulse[LEN_LINES];
    char* next[LEN_LINES];
    size_t next_count;
} conjunction_t;

typedef struct {
    module_type_t type;
    union {
        broadercaster_t broadercaster;
        flip_flop_t flip_flop;
        conjunction_t conjunction;
    } module;
} module_t;

typedef struct {
    char* from;
    module_t* module;
    pulse_t signal;
} module_frame_t;

typedef struct {
    module_frame_t q[LEN_LINES];
    size_t q_begin;
    size_t q_end;
} module_queue_t;

void queue_push(module_queue_t* q, module_frame_t module) {
    q->q[q->q_end] = module;
    q->q_end += 1;
    q->q_end %= LEN_LINES;
}

void queue_pop(module_queue_t* q) {
    q->q_begin += 1;
    q->q_begin %= LEN_LINES;
}

module_frame_t queue_front(module_queue_t* q) {
    return q->q[q->q_begin];
}

size_t queue_count(module_queue_t* q) {
    size_t x = q->q_begin;
    size_t y = q->q_end;

    if (x > y) {
        y += LEN_LINES;
    }

    return y - x;
}

module_t modules[LEN_LINES] = { 0, };
size_t module_count = 0;

module_t* find_module(char* name) {
    for (size_t i = 0; i < module_count; i++) {
        module_t* module = &modules[i];
        broadercaster_t* broad = &module->module.broadercaster;
        flip_flop_t* flip = &module->module.flip_flop;
        conjunction_t* con = &module->module.conjunction;

        switch (module->type) {
        case BROADCASTER:
            if (strcmp(broad->name, name) == 0) return module;
            break;
        case FLIP_FLOP:
            if (strcmp(flip->name, name) == 0) return module;
            break;
        case CONJUNCTION:
            if (strcmp(con->name, name) == 0) return module;
            break;
        }
    }

    return NULL;
}

void print_module(module_t* module, size_t depth) {
    if (depth > 1) {
        return;
    }
    if (module == NULL) {
        for (size_t i = 0; i < depth; i++) printf("  ");
        printf("this may be the output\n");
        return;
    }

    broadercaster_t* broad = &module->module.broadercaster;
    flip_flop_t* flip = &module->module.flip_flop;
    conjunction_t* con = &module->module.conjunction;
    module_t* next = NULL;

    switch (module->type) {
    case BROADCASTER:
        for (size_t i = 0; i < depth; i++) printf("  ");
        printf("BROADCASTER: '%s'\n", broad->name);
        for (size_t i = 0; i < depth; i++) printf("  ");
        printf("next (%zu):\n", broad->next_count);
        for (size_t j = 0; j < broad->next_count; j++) {
            next = find_module(broad->next[j]);
            print_module(next, depth + 1);
        }
        break;
    case FLIP_FLOP:
        for (size_t i = 0; i < depth; i++) printf("  ");
        printf("FLIP FLOP: '%s'\n", flip->name);
        for (size_t i = 0; i < depth; i++) printf("  ");
        printf("next (%zu):\n", flip->next_count);
        for (size_t j = 0; j < flip->next_count; j++) {
            next = find_module(flip->next[j]);
            print_module(next, depth + 1);
        }
        break;
    case CONJUNCTION:
        for (size_t i = 0; i < depth; i++) printf("  ");
        printf("CONJUNCTION: '%s'\n", con->name);
        for (size_t i = 0; i < depth; i++) printf("  ");
        printf("inputs (%zu):\n", con->input_count);
        for (size_t j = 0; j < con->input_count; j++) {
            next = find_module(con->inputs[j]);
            print_module(next, depth + 1);
        }
        for (size_t i = 0; i < depth; i++) printf("  ");
        printf("next (%zu):\n", con->next_count);
        for (size_t j = 0; j < con->next_count; j++) {
            next = find_module(con->next[j]);
            print_module(next, depth + 1);
        }
        break;
    }
}

void update_inputs(module_t* module, char* name, pulse_t signal) {
    if (module->type != CONJUNCTION) {
        return;
    }

    conjunction_t* con = &module->module.conjunction;

    for (size_t i = 0; i < con->input_count; i++) {
        if (strcmp(con->inputs[i], name) == 0) {
            con->last_pulse[i] = signal;
            break;
        }
    }
}

void send_signal(module_t* begin, pulse_t signal, size_t* num_low, size_t* num_high, bool count_only_end, size_t count) {
    module_queue_t q = { 0, };
    queue_push(&q, (module_frame_t){ .from = "broadercaster", .module = begin, .signal = signal });

    while (queue_count(&q) > 0) {
        module_frame_t module_frame = queue_front(&q);
        queue_pop(&q);

        char* prev_name = module_frame.from;
        module_t* module = module_frame.module;
        pulse_t signal = module_frame.signal;

        if (!count_only_end) {
            *num_low += signal == LOW;
            *num_high += signal == HIGH;
        }

        if (module == NULL) {
            if (count_only_end) {
                *num_low += signal == LOW;
                *num_high += signal == HIGH;
            }
            continue;
        }

        broadercaster_t* broad = &module->module.broadercaster;
        flip_flop_t* flip = &module->module.flip_flop;
        conjunction_t* con = &module->module.conjunction;
        module_t* next = NULL;
        bool send_low = true;

        switch (module->type) {
        case BROADCASTER:
            for (size_t i = 0; i < broad->next_count; i++) {
                next = find_module(broad->next[i]);
                queue_push(&q, (module_frame_t){ .from = broad->name, .module = next, .signal = signal });
            }
            break;
        case FLIP_FLOP:
            if (signal == HIGH) {
                break;
            }

            flip->is_on = !flip->is_on;
            signal = flip->is_on ? HIGH : LOW;

            for (size_t i = 0; i < flip->next_count; i++) {
                next = find_module(flip->next[i]);
                queue_push(&q, (module_frame_t){ .from = flip->name, .module = next, .signal = signal });
            }
            break;
        case CONJUNCTION:
            if (strcmp(con->name, "dd") == 0 && signal == HIGH) {
                printf("%s: %d: %zu\n", prev_name, signal, count);
            }

            update_inputs(module, prev_name, signal);

            for (size_t i = 0; i < con->input_count; i++) {
                send_low &= con->last_pulse[i] == HIGH;
            }
            signal = send_low ? LOW : HIGH;

            for (size_t i = 0; i < con->next_count; i++) {
                next = find_module(con->next[i]);
                queue_push(&q, (module_frame_t){ .from = con->name, .module = next, .signal = signal });
            }
            break;
        }
    }
}

size_t part_one(string_t* lines, size_t len_lines) {
    for (size_t i = 0; i < len_lines; i++) {
        module_t* curr = &modules[module_count++];

        char* name = strtok(lines[i].str, " ");
        curr->type = module_type_lookup[*name];
        char* token = strtok(NULL, " ");
        token += strlen(token) + 1;

        broadercaster_t* broad = &curr->module.broadercaster;
        flip_flop_t* flip = &curr->module.flip_flop;
        conjunction_t* con = &curr->module.conjunction;

        switch (curr->type) {
        case BROADCASTER:
            broad->name = name;
            while ((token = strtok(NULL, ",")) != NULL) {
                broad->next[broad->next_count++] = token + (*token == ' ');
            }
            break;
        case FLIP_FLOP:
            flip->name = name + 1;
            while ((token = strtok(NULL, ",")) != NULL) {
                flip->next[flip->next_count++] = token + (*token == ' ');
            }
            break;
        case CONJUNCTION:
            con->name = name + 1;
            while ((token = strtok(NULL, ",")) != NULL) {
                con->next[con->next_count++] = token + (*token == ' ');
            }
            break;
        }
    }

    for (size_t i = 0; i < module_count; i++) {
        module_t* curr = &modules[i];
        module_t* next = NULL;

        broadercaster_t* broad = &curr->module.broadercaster;
        flip_flop_t* flip = &curr->module.flip_flop;
        conjunction_t* con = &curr->module.conjunction;

        switch (curr->type) {
        case BROADCASTER:
            for (size_t i = 0; i < broad->next_count; i++) {
                next = find_module(broad->next[i]);
                if (next != NULL && next->type == CONJUNCTION) {
                    size_t* count = &next->module.conjunction.input_count;
                    next->module.conjunction.inputs[*count] = broad->name;
                    *count += 1;
                }
            }
            break;
        case FLIP_FLOP:
            for (size_t i = 0; i < flip->next_count; i++) {
                next = find_module(flip->next[i]);
                if (next != NULL && next->type == CONJUNCTION) {
                    size_t* count = &next->module.conjunction.input_count;
                    next->module.conjunction.inputs[*count] = flip->name;
                    *count += 1;
                }
            }
            break;
        case CONJUNCTION:
            for (size_t i = 0; i < con->next_count; i++) {
                next = find_module(con->next[i]);
                if (next != NULL && next->type == CONJUNCTION) {
                    size_t* count = &next->module.conjunction.input_count;
                    next->module.conjunction.inputs[*count] = con->name;
                    *count += 1;
                }
            }
            break;
        }
    }

    module_t* begin = find_module("broadcaster");
    size_t num_low = 0;
    size_t num_high = 0;

    for (size_t i = 0; i < 1000; i++) {
        send_signal(begin, LOW, &num_low, &num_high, false, i);
    }

    size_t answer = num_low * num_high;

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    for (size_t i = 0; i < len_lines; i++) {
        module_t* curr = &modules[module_count++];

        char* name = strtok(lines[i].str, " ");
        curr->type = module_type_lookup[*name];
        char* token = strtok(NULL, " ");
        token += strlen(token) + 1;

        broadercaster_t* broad = &curr->module.broadercaster;
        flip_flop_t* flip = &curr->module.flip_flop;
        conjunction_t* con = &curr->module.conjunction;

        switch (curr->type) {
        case BROADCASTER:
            broad->name = name;
            while ((token = strtok(NULL, ",")) != NULL) {
                broad->next[broad->next_count++] = token + (*token == ' ');
            }
            break;
        case FLIP_FLOP:
            flip->name = name + 1;
            while ((token = strtok(NULL, ",")) != NULL) {
                flip->next[flip->next_count++] = token + (*token == ' ');
            }
            break;
        case CONJUNCTION:
            con->name = name + 1;
            while ((token = strtok(NULL, ",")) != NULL) {
                con->next[con->next_count++] = token + (*token == ' ');
            }
            break;
        }
    }

    for (size_t i = 0; i < module_count; i++) {
        module_t* curr = &modules[i];
        module_t* next = NULL;

        broadercaster_t* broad = &curr->module.broadercaster;
        flip_flop_t* flip = &curr->module.flip_flop;
        conjunction_t* con = &curr->module.conjunction;

        switch (curr->type) {
        case BROADCASTER:
            for (size_t i = 0; i < broad->next_count; i++) {
                next = find_module(broad->next[i]);
                if (next != NULL && next->type == CONJUNCTION) {
                    size_t* count = &next->module.conjunction.input_count;
                    next->module.conjunction.inputs[*count] = broad->name;
                    *count += 1;
                }
            }
            break;
        case FLIP_FLOP:
            for (size_t i = 0; i < flip->next_count; i++) {
                next = find_module(flip->next[i]);
                if (next != NULL && next->type == CONJUNCTION) {
                    size_t* count = &next->module.conjunction.input_count;
                    next->module.conjunction.inputs[*count] = flip->name;
                    *count += 1;
                }
            }
            break;
        case CONJUNCTION:
            for (size_t i = 0; i < con->next_count; i++) {
                next = find_module(con->next[i]);
                if (next != NULL && next->type == CONJUNCTION) {
                    size_t* count = &next->module.conjunction.input_count;
                    next->module.conjunction.inputs[*count] = con->name;
                    *count += 1;
                }
            }
            break;
        }
    }

    module_t* begin = find_module("broadcaster");
    size_t count = 1;

    while (true) {
        size_t num_low = 0;
        size_t num_high = 0;

        send_signal(begin, LOW, &num_low, &num_high, true, count);

        // printf("%zu, %zu\n", num_low, num_high);

        if (num_low == 1 && num_high == 0) {
            return count;
        }

        count++;
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
