#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define LEN_LINES 1200
#define LEN_LINE 1200

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

typedef enum {
    NONE,
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
} deck_kind_t;

char* deck_kinds[] = {
    "NONE",
    "HIGH_CARD",
    "ONE_PAIR",
    "TWO_PAIR",
    "THREE_OF_A_KIND",
    "FULL_HOUSE",
    "FOUR_OF_A_KIND",
    "FIVE_OF_A_KIND"
};

typedef struct {
    char cards[LEN_LINE];
    deck_kind_t kind;
    size_t factor;
} deck_t;

int comp_size_desc(const void* p, const void* q) {
    size_t i = *(size_t*)p;
    size_t j = *(size_t*)q;

    return -((i > j) - (i < j));
}

int comp_cards(const void* p, const void* q) {
    static int lookup[128] = {
        ['A'] = 4,
        ['K'] = 3,
        ['Q'] = 2,
        ['J'] = 1,
        ['T'] = 0,
    };

    char* i = (char*)p;
    char* j = (char*)q;

    while (*i != '\0') {
#ifdef PART_TWO
        if (*i == 'J' && *j != 'J') {
            return -1;
        } else if (*i != 'J' && *j == 'J') {
            return 1;
        }
#endif

        if (isupper(*i) && isdigit(*j)) {
            return 1;
        } else if (isdigit(*i) && isupper(*j)) {
            return -1;
        } else if (isdigit(*i) && isdigit(*j)) {
            if (*i != *j) {
                return (*i > *j) - (*i < *j);
            }
        } else {
            if (*i != *j) {
                return (lookup[*i] > lookup[*j]) - (lookup[*i] < lookup[*j]);
            }
        }

        i++;
        j++;
    }

    return 0;
}

int comp_deck(const void* p, const void* q) {
    deck_t* i = (deck_t*)p;
    deck_t* j = (deck_t*)q;

    if (i->kind == j->kind) {
        return comp_cards(i->cards, j->cards);
    }

    return (i->kind > j->kind) - (i->kind < j->kind);
}

deck_kind_t calc_deck_kind(deck_t* ths) {
    size_t lookup[128] = { 0, };

    char* cards = ths->cards;
    while (*cards != '\0') {
        lookup[*cards]++;
        cards++;
    }

    qsort(lookup, 128, sizeof(size_t), comp_size_desc);

    if (lookup[0] == 5) {
        return FIVE_OF_A_KIND;
    } else if (lookup[0] == 4) {
        return FOUR_OF_A_KIND;
    } else if (lookup[0] == 3 && lookup[1] == 2) {
        return FULL_HOUSE;
    } else if (lookup[0] == 3) {
        return THREE_OF_A_KIND;
    } else if (lookup[0] == 2 && lookup[1] == 2) {
        return TWO_PAIR;
    } else if (lookup[0] == 2) {
        return ONE_PAIR;
    }

    return HIGH_CARD;
}

deck_kind_t calc_deck_kind_with_joker(deck_t* ths) {
    size_t lookup[128] = { 0, };

    char* cards = ths->cards;
    while (*cards != '\0') {
        lookup[*cards]++;
        cards++;
    }

    size_t jokers = lookup['J'];
    lookup['J'] = 0;

    qsort(lookup, 128, sizeof(size_t), comp_size_desc);

    if (lookup[0] == 5 - jokers) {
        return FIVE_OF_A_KIND;
    } else if (lookup[0] == 4 - jokers) {
        return FOUR_OF_A_KIND;
    } else if (lookup[0] + lookup[1] + jokers == 5) {
        return FULL_HOUSE;
    } else if (lookup[0] == 3 - jokers) {
        return THREE_OF_A_KIND;
    } else if (lookup[0] + lookup[1] + jokers == 4) {
        return TWO_PAIR;
    } else if (lookup[0] == 2 - jokers) {
        return ONE_PAIR;
    }

    return HIGH_CARD;
}

size_t part_one(string_t* lines, size_t len_lines) {
    deck_t decks[LEN_LINES] = { 0, };
    size_t deck_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* cards = strtok(lines[i].str, " ");
        char* factor_str = strtok(NULL, " ");
        size_t factor = strtoll(factor_str, NULL, 10);

        strcpy(decks[deck_count].cards, cards);
        decks[deck_count].kind = calc_deck_kind(&decks[deck_count]);
        decks[deck_count].factor = factor;
        deck_count++;
    }

    qsort(decks, deck_count, sizeof(deck_t), comp_deck);

    size_t answer = 0;
    for (size_t i = 0; i < deck_count; i++) {
        answer += (i + 1) * decks[i].factor;
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    deck_t decks[LEN_LINES] = { 0, };
    size_t deck_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* cards = strtok(lines[i].str, " ");
        char* factor_str = strtok(NULL, " ");
        size_t factor = strtoll(factor_str, NULL, 10);

        strcpy(decks[deck_count].cards, cards);
        decks[deck_count].kind = calc_deck_kind_with_joker(&decks[deck_count]);
        decks[deck_count].factor = factor;
        deck_count++;
    }

    qsort(decks, deck_count, sizeof(deck_t), comp_deck);

    size_t answer = 0;
    for (size_t i = 0; i < deck_count; i++) {
        answer += (i + 1) * decks[i].factor;
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
