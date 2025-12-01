def part1(input_str):
    ans = 0
    dial = 50
    for line in input_str:
        d = 1 if line[0] == 'R' else -1
        i = int(line[1:])

        dial = (dial + d * i) % 100

        if dial == 0:
            ans += 1

    return ans

def part2(input_str):
    ans = 0
    dial = 50
    for line in input_str:
        d = 1 if line[0] == 'R' else -1
        i = int(line[1:])

        naive = False
        if naive:
            for j in range(i):
                dial = (dial + d) % 100
                if dial == 0:
                    ans += 1
        else:
            prev_dial = dial
            dial = (dial + d * i) % 100

            rot = i // 100
            if prev_dial != 0 and dial != 0:
                if d == 1 and prev_dial > dial:
                    rot += 1
                elif d == -1 and prev_dial < dial:
                    rot += 1
            elif prev_dial != 0 and dial == 0:
                rot += 1

            ans += rot

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
