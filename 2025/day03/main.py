from functools import cache
from itertools import combinations

def part1(input_str):
    ans = 0
    for line in input_str:
        mx = -1
        for a, b in combinations(line, 2):
            mx = max(mx, int(a + b))

        ans += mx

    return ans

def part2(input_str):
    @cache
    def find_maximum_of_len(digits, index, length):
        if index >= len(digits):
            return 0 if length == 0 else -float('inf')
        elif length == 0:
            return 0

        take = int(digits[index]) * 10**(length - 1) + find_maximum_of_len(digits, index + 1, length - 1)
        no_take = find_maximum_of_len(digits, index + 1, length)

        return max(take, no_take)

    ans = 0
    for line in input_str:
        ans += find_maximum_of_len(line, 0, 12)

    return ans

if __name__ == '__main__':
    input_str = []
    while True:
        try:
            input_str.append(input())
        except:
            break

    print(f'Part 1: {part1(input_str)}')
    print(f'Part 2: {part2(input_str)}')
