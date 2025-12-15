from collections import defaultdict, deque

def part1(input_str):
    adj = defaultdict(list)
    for line in input_str:
        src, dests = line.split(':')
        dests = dests.split()

        for dest in dests:
            adj[src].append(dest)

    q = deque()
    q.append(('you'))

    ans = 0
    while q:
        curr = q.popleft()
        if curr == 'out':
            ans += 1
            continue

        for nxt in adj[curr]:
            q.append(nxt)

    return ans

def part2(input_str):
    return 0

if __name__ == '__main__':
    input_str = []
    while True:
        try:
            input_str.append(input())
        except:
            break

    print(f'Part 1: {part1(input_str)}')
    print(f'Part 2: {part2(input_str)}')
