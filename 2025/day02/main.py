import re

def part1(input_str):
    lines = ''.join(input_str).split(',')
    half = re.compile(r'^(\d+?)\1$')

    ans = 0
    for rng in lines:
        begin, end = map(int, rng.split('-'))
        for i in range(begin, end + 1):
            if half.match(str(i)):
                ans += i

    return ans

def part2(input_str):
    lines = ''.join(input_str).split(',')
    regex = re.compile(r'^(\d+?)\1+$')

    ans = 0
    for rng in lines:
        begin, end = map(int, rng.split('-'))
        for i in range(begin, end + 1):
            if regex.match(str(i)):
                ans += i

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
