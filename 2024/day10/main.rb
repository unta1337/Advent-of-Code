#!/usr/bin/env ruby

input = File.open("input.txt", "r").map { |row| row.strip.chars.map(&:to_i) }

def calc_score(field, row, col, enable_visited_check = true)
  ret = 0
  q = Queue.new
  visited = Array.new(field.size) { Array.new(field[0].size) }

  q.push [row, col]
  visited[row][col] = true

  while !q.empty?
    r, c = q.pop
    if field[r][c] == 9
      ret += 1
    end

    [[-1, 0], [1, 0], [0, -1], [0, 1]].each do |dr, dc|
      nr, nc = r + dr, c + dc
      if nr.between?(0, field.size - 1) && nc.between?(0, field[0].size - 1) &&
         !visited[nr][nc] && field[nr][nc] - field[r][c] == 1
        q.push [nr, nc]
        if enable_visited_check
          visited[nr][nc] = true
        end
      end
    end
  end

  ret
end

# Part 1
part1 = 0
(0...input.size).each do |i|
  (0...input[0].size).each do |j|
    if input[i][j] == 0
      part1 += calc_score input, i, j
    end
  end
end

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = 0
(0...input.size).each do |i|
  (0...input[0].size).each do |j|
    if input[i][j] == 0
      part2 += calc_score input, i, j, false
    end
  end
end

puts "== Part 2 =="
puts part2
