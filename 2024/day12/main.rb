#!/usr/bin/env ruby

LEFT = 1
RIGHT = 2
UP = 4
DOWN = 8

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

def mark_part2(field, row, col, visited, target = nil)
  if target.nil?
    target = field[row][col]
  end

  field[row][col] = "."
  visited[row][col] = true
  area = 1
  corner = 0

  [[-1, 0], [1, 0], [0, -1], [0, 1]].each.with_index do |(dr, dc), i|
    nr, nc = row + dr, col + dc
    if nr.between?(0, field.size - 1) && nc.between?(0, field[0].size - 1)
      if !visited[nr][nc] && field[nr][nc] == target
        aa, cc = mark_part2 field, nr, nc, visited, target
        area += aa
        corner += cc
      end
    end
  end

  [area, corner]
end

def fill(field, row, col, to, target = nil)
  if target.nil?
    target = field[row][col]
  end

  field[row][col] = to
  [[-1, 0], [1, 0], [0, -1], [0, 1]].each.with_index do |(dr, dc), i|
    nr, nc = row + dr, col + dc
    if nr.between?(0, field.size - 1) && nc.between?(0, field[0].size - 1) && field[nr][nc] == target
      fill field, nr, nc, to, target
    end
  end
end

def get_corners(field)
  ret = 0
  (-1..field.size - 1).each do |i|
    (-1..field[0].size - 1).each do |j|
      foo = 0
      bar = 0
      [[0, 0], [0, 1], [1, 0], [1, 1]].each.with_index do |(dr, dc), k|
        nr, nc = i + dr, j + dc
        if nr.between?(0, field.size - 1) && nc.between?(0, field[0].size - 1) && field[nr][nc] == "."
          foo += 1
          bar |= 1 << k
        end
      end

      if foo == 1
        ret += 1
      elsif foo == 3
        ret += 1
      elsif foo == 2
        if bar == 6 || bar == 9
          ret += 2
        end
      end
    end
  end

  ret
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
field = input.deep_clone
visited = Array.new(rows) { Array.new(cols, false) }

part2 = 0
(0...rows).each do |i|
  (0...cols).each do |j|
    if !visited[i][j]
      bak = field[i][j]

      aa, _ = mark_part2 field, i, j, visited
      cc = get_corners field

      fill field, i, j, bak

      part2 += aa * cc
    end
  end
end

puts "== Part 2 =="
puts part2
