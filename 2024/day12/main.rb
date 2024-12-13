#!/usr/bin/env ruby

class Array
  def deep_clone
    self.map { |e| e.class == Array ? e.clone : e }
  end
end

input = File.open("input.txt", "r").map { |row| row.strip }
rows = input.size
cols = input[0].size

def mark(field, row, col, visited, target = nil)
  if target.nil?
    target = field[row][col]
  end

  visited[row][col] = true
  area = 1
  peri = 0

  [[-1, 0], [1, 0], [0, -1], [0, 1]].each do |dr, dc|
    nr, nc = row + dr, col + dc
    if nr.between?(0, field.size - 1) && nc.between?(0, field[0].size - 1)
      if !visited[nr][nc] && field[nr][nc] == target
        aa, pp = mark field, nr, nc, visited, target
        area += aa
        peri += pp
      elsif field[nr][nc] != target
        peri += 1
      end
    else
      peri += 1
    end
  end

  [area, peri]
end

# Part 1
field = input.deep_clone
visited = Array.new(rows) { Array.new(cols, false) }

part1 = 0
(0...rows).each do |i|
  (0...cols).each do |j|
    if !visited[i][j]
      aa, pp = mark field, i, j, visited
      part1 += aa * pp
    end
  end
end

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = 0

puts "== Part 2 =="
puts part2
