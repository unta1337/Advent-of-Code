from functools import cache

def part1(input_str):
    grid = [list(line) for line in input_str]
    n = len(grid)
    m = len(grid[0])

    ans = 0
    def dfs(row, col):
        nonlocal ans

        if row >= n:
            return

        if grid[row][col] == 'S' or grid[row][col] == '.':
            grid[row][col] = '|'
            dfs(row + 1, col)
        elif grid[row][col] == '^':
            ans += 1
            dfs(row, col + 1)
            dfs(row, col - 1)
    dfs(0, grid[0].index('S'))

    return ans

def part2(input_str):
    grid = [list(line) for line in input_str]
    n = len(grid)
    m = len(grid[0])
    node_map = [[float('inf') for _ in range(m)] for _ in range(n + 1)]
    node_locs = []
    node_count = 0

    for i in range(n):
        for j in range(m):
            if grid[i][j] == 'S' or grid[i][j] == '^':
                node_map[i][j] = node_count
                node_locs.append((i, j))
                node_count += 1
    for j in range(m):
        node_map[i][j] = node_count
        node_locs.append((n, j))
        node_count += 1
    for j in range(m):
        for i in range(n - 2, -1, -1):
            node_map[i][j] = min(node_map[i][j], node_map[i + 1][j])

    adj = [[] for _ in range(node_count + 1)]
    for row, col in node_locs:
        curr = node_map[row][col]
        if curr == 0:
            below = node_map[row + 1][col] if row + 1 <= n else float('inf')

            if below < float('inf'):
                adj[curr].append(below)

            continue


        left = node_map[row][col - 1] if col - 1 >= 0 else float('inf')
        right = node_map[row][col + 1] if col + 1 < m else float('inf')

        if left < float('inf'):
            adj[curr].append(left)
        if right < float('inf'):
            adj[curr].append(right)

    @cache
    def dfs(index):
        if not adj[index]:
            return 1

        return sum(dfs(nxt) for nxt in adj[index])

    return dfs(0)

if __name__ == '__main__':
    input_str = []
    while True:
        try:
            input_str.append(input())
        except:
            break

    print(f'Part 1: {part1(input_str)}')
    print(f'Part 2: {part2(input_str)}')
