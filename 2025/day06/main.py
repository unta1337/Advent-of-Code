import re

from functools import reduce

def part1(input_str):
    cols = len(input_str[0].split())
    xss = [[] for _ in range(cols)]

    ans = 0
    for line in input_str:
        ys = line.split()
        if ys[0].isdigit():
            for xs, y in zip(xss, ys):
                xs.append(int(y))
        else:
            for xs, y in zip(xss, ys):
                if y == '+':
                    f = lambda p, q: p + q
                elif y == '*':
                    f = lambda p, q: p * q
                ans += reduce(f, xs)

    return ans

def part2(input_str):
    ops = input_str[-1].split()
    op_count = len(ops)

    char_mat = [list(line) for line in input_str[:-1]]
    char_mat = [list(col) for col in zip(*char_mat)] + [' ']

    ans = 0
    op_index = 0
    xs = []
    for row in char_mat:
        if all(cell == ' ' for cell in row):
            op = ops[op_index]
            op_index += 1

            if op == '+':
                f = lambda p, q: p + q
            elif op == '*':
                f = lambda p, q: p * q
            ans += reduce(f, xs)

            xs = []
            continue

        xs.append(int(''.join(d for d in row if d != '')))

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
