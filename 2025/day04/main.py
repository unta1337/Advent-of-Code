from itertools import product

def part1(input_str):
    n = len(input_str)
    m = len(input_str[0])

    ans = 0
    for i in range(n):
        for j in range(m):
            if input_str[i][j] != '@':
                continue

            neighbor_count = 0
            for dr, dc in filter(lambda e: e[0] != 0 or e[1] != 0, product([-1, 0, 1], [-1, 0, 1])):
                nr, nc = i + dr, j + dc
                cell = input_str[nr][nc] if 0 <= nr < n and 0 <= nc < m else '.'
                neighbor_count += 1 if cell == '@' else 0

            ans += 1 if neighbor_count < 4 else 0

    return ans

def part2(input_str):
    papers = [list(line) for line in input_str]
    n = len(input_str)
    m = len(input_str[0])

    ans = 0
    while True:
        count = 0
        for i in range(n):
            for j in range(m):
                if papers[i][j] != '@':
                    continue

                neighbor_count = 0
                for dr, dc in filter(lambda e: e[0] != 0 or e[1] != 0, product([-1, 0, 1], [-1, 0, 1])):
                    nr, nc = i + dr, j + dc
                    cell = papers[nr][nc] if 0 <= nr < n and 0 <= nc < m else '.'
                    neighbor_count += 1 if cell != '.' else 0

                if neighbor_count < 4:
                    count += 1
                    papers[i][j] = 'x'

        for i in range(n):
            for j in range(m):
                if papers[i][j] == 'x':
                    papers[i][j] = '.'

        if count == 0:
            break
        ans += count

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
