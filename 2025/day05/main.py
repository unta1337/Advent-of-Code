def part1(input_str):
    ranges = []

    i = 0
    while input_str[i] != '':
        begin, end = map(int, input_str[i].split('-'))
        ranges.append(range(begin, end + 1))
        i += 1
    i += 1

    ans = 0
    while i < len(input_str):
        x = int(input_str[i])
        if any(x in rng for rng in ranges):
            ans += 1
        i += 1

    return ans

def part2(input_str):
    ranges = []
    for line in input_str:
        if line == '':
            break

        begin, end = map(int, line.split('-'))
        ranges.append(range(begin, end + 1))
    ranges.sort(key=lambda e: (e.start, e.stop))

    merged = []
    for rng in ranges:
        if not merged or rng.start >= merged[-1].stop:
            merged.append(rng)
        else:
            merged[-1] = range(merged[-1].start, max(merged[-1].stop, rng.stop))

    return sum(map(lambda e: len(e), merged))

if __name__ == '__main__':
    input_str = []
    while True:
        try:
            input_str.append(input())
        except:
            break

    print(f'Part 1: {part1(input_str)}')
    print(f'Part 2: {part2(input_str)}')
