from functools import reduce

def part1(input_str):
    boxes = [tuple(map(int, line.split(','))) for line in input_str]
    n = len(boxes)

    edges = []
    for i in range(n):
        for j in range(i + 1, n):
            x1, y1, z1 = boxes[i]
            x2, y2, z2 = boxes[j]

            dx = x1 - x2
            dy = y1 - y2
            dz = z1 - z2

            d_sq = dx * dx + dy * dy + dz * dz

            edges.append((d_sq, i, j))
    edges.sort()

    groups = [None for _ in range(n)]
    group_size = [1 for _ in range(n)]

    def find(i):
        if groups[i] is None:
            return i

        root = find(groups[i])
        groups[i] = root
        group_size[root] += group_size[i]
        group_size[i] = 0

        return root

    def merge(i, j):
        i = find(i)
        j = find(j)

        if i != j:
            groups[j] = i
            group_size[i] += group_size[j]
            group_size[j] = 0

    for _, i, j in edges[:1000 if n == 1000 else 10]:
        i_root = find(i)
        j_root = find(j)

        if i_root != j_root:
            merge(i, j)

    return reduce(lambda p, q: p * q, sorted(group_size)[-3:])

def part2(input_str):
    boxes = [tuple(map(int, line.split(','))) for line in input_str]
    n = len(boxes)

    edges = []
    for i in range(n):
        for j in range(i + 1, n):
            x1, y1, z1 = boxes[i]
            x2, y2, z2 = boxes[j]

            dx = x1 - x2
            dy = y1 - y2
            dz = z1 - z2

            d_sq = dx * dx + dy * dy + dz * dz

            edges.append((d_sq, i, j))
    edges.sort()

    groups = [None for _ in range(n)]
    group_size = [1 for _ in range(n)]

    def find(i):
        if groups[i] is None:
            return i

        root = find(groups[i])
        groups[i] = root
        group_size[root] += group_size[i]
        group_size[i] = 0

        return root

    def merge(i, j):
        i = find(i)
        j = find(j)

        if i != j:
            groups[j] = i
            group_size[i] += group_size[j]
            group_size[j] = 0

    ret = None
    for _, i, j in edges:
        i_root = find(i)
        j_root = find(j)

        if i_root != j_root:
            merge(i_root, j_root)

            if group_size[i_root] == n:
                ret = boxes[i][0] * boxes[j][0]
                break

    return ret

if __name__ == '__main__':
    input_str = []
    while True:
        try:
            input_str.append(input())
        except:
            break

    print(f'Part 1: {part1(input_str)}')
    print(f'Part 2: {part2(input_str)}')
