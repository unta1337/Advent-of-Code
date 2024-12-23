#!/usr/bin/env ruby

$sample = false
input_path = $sample ? "sample.txt" : "input.txt"

input = File.open(input_path, "r").map(&:strip)
rows = input.size
cols = input[0].size
src = (0...rows).to_a.product((0...cols).to_a).find { |r, c| input[r][c] == "S" }
dest = (0...rows).to_a.product((0...cols).to_a).find { |r, c| input[r][c] == "E" }

# Part 1
q = Queue.new
lookup = Array.new(rows) { Array.new(cols, Float::INFINITY) }

q.push [*src]
lookup[src[0]][src[1]] = 0

no_cheat = nil
steps = 0

while !q.empty?
  iter = q.size
  iter.times do
    row, col = q.pop
    if [row, col] == dest
      no_cheat = steps
      next
    end

    [[-1, 0], [1, 0], [0, -1], [0, 1]].each do |dr, dc|
      nr, nc = row + dr, col + dc
      if nr.between?(0, rows - 1) && nc.between?(0, cols - 1) && steps + 1 <= lookup[nr][nc] && input[nr][nc] != "#"
        q.push [nr, nc]
        lookup[nr][nc] = steps + 1
      end
    end
  end

  steps += 1
end

visited = Array.new(rows) { Array.new(cols) { Array.new(5, false) } }

q.push [*src, 2]
visited[src[0]][src[1]][4] = true

part1 = 0
steps = 0

while !q.empty?
  iter = q.size
  iter.times do
    row, col, t = q.pop
    if t == 0
      diff = no_cheat - lookup[row][col]
      cheat = steps + diff
      if no_cheat - cheat >= 100
        part1 += 1
      end
      next
    end

    [[-1, 0], [1, 0], [0, -1], [0, 1]].each_with_index do |(dr, dc), i|
      nr, nc = row + dr, col + dc
      if nr.between?(0, rows - 1) && nc.between?(0, cols - 1)
        if t == 2
          if input[nr][nc] != "#" && !visited[nr][nc][i]
            q.push [nr, nc, t]
            visited[nr][nc][i] = true
          elsif input[nr][nc] == "#" && !visited[nr][nc][i]
            q.push [nr, nc, t - 1]
            visited[nr][nc][i] = true
          end
        elsif t == 1
          if input[nr][nc] != "#" && !visited[nr][nc][i]
            q.push [nr, nc, t - 1]
            visited[nr][nc][i] = true
          end
        elsif t == 0
          if input[nr][nc] != "#" && !visited[nr][nc][i]
            q.push [nr, nc, t]
            visited[nr][nc][i] = true
          end
        end
      end
    end
  end

  steps += 1
end

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = 0

puts "== Part 2 =="
puts part2
