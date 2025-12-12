def part1(input_str):
    ans = 0
    for line in input_str:
        target, *rest = line.split()
        buttons, _ = rest[:-1], rest[-1]

        target = tuple(cell == '#' for cell in target[1:-1])
        n = len(target)
        buttons = [tuple(map(int, e[1:-1].split(','))) for e in buttons]

        push_count = 0
        inital_state = tuple(False for _ in range(n))
        q = [inital_state]
        visited = set([inital_state])

        try:
            while True:
                if not q:
                    raise Exception(0)

                qq = []
                for state in q:
                    if state == target:
                        raise Exception(push_count)

                    for button in buttons:
                        next_state = list(state)
                        for press in button:
                            next_state[press] = not next_state[press]
                        next_state = tuple(next_state)

                        if next_state not in visited:
                            qq.append(next_state)
                            visited.add(next_state)

                q = qq
                push_count += 1
        except Exception as e:
            ans += e.args[0]

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
