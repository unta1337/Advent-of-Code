from itertools import combinations

def part1(input_str):
    reds = [tuple(map(int, line.split(','))) for line in input_str]

    ans = 0
    for p, q in combinations(reds, 2):
        dx = (p[0] - q[0]) + 1
        dy = (p[1] - q[1]) + 1
        area = dx * dy

        ans = max(ans, area)

    return ans

def part2(input_str):
    reds = [tuple(map(int, line.split(','))) for line in input_str]
    n = len(reds)

    def get_dir(prev, curr):
        dx = curr[0] - prev[0]
        dy = curr[1] - prev[1]

        if dy > 0:
            return 'down'
        elif dy < 0:
            return 'up'
        elif dx > 0:
            return 'right'
        elif dx < 0:
            return 'left'

        assert False and "Unreachable"

    vertical = []
    horizontal = []
    for i in range(n):
        prev = reds[i - 1]
        curr = reds[i]
        direction = get_dir(prev, curr)

        if direction == 'up' or direction == 'down':
            vertical.append((min(prev, curr), max(prev, curr)))
        else:
            horizontal.append((min(prev, curr), max(prev, curr)))

    ans = 0
    for l1, l2 in combinations(vertical, 2):
        x_begin = min(l1[0][0], l2[0][0])
        x_end = max(l1[0][0], l2[0][0])
        width = x_end - x_begin + 1

        y_begin = max(l1[0][1], l2[0][1])
        y_end = min(l1[1][1], l2[1][1])
        height = y_end - y_begin + 1
        if height <= 0:
            continue

        for l3 in horizontal:
            if l3[0][0] > x_end or l3[1][0] < x_begin:
                continue

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
