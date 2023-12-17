#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>

#define BUF_LEN 200
#define RES_LEN 4000

int comp(const void* p, const void* q) {
    int i = *(int*)p;
    int j = *(int*)q;

    return j - i;
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

    int results[RES_LEN] = { 0, };
    size_t result_index = 0;

    int elf_sum = 0;
    int answer = 0;

    for (size_t line_count = 1; fgets(buffer, BUF_LEN, fp) != NULL; line_count++) {
        // remove line feed character.
        size_t len = strlen(buffer);
        buffer[--len] = '\0';

        // do your thing.
        if (len > 0) {
            elf_sum += strtol(buffer, NULL, 10);
        } else {
            results[result_index++] = elf_sum;
            answer = fmax(answer, elf_sum);
            elf_sum = 0;
        }
    }
    results[result_index++] = elf_sum;
    answer = fmax(answer, elf_sum);
    elf_sum = 0;

#ifdef PART_TWO
    qsort(results, result_index, sizeof(int), comp);
    answer = results[0] + results[1] + results[2];
#endif

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
