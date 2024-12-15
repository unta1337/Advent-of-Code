#!/usr/bin/env ruby

$sample = false
input_path = $sample ? "sample.txt" : "input.txt"

input = File.open(input_path, "r").map(&:strip)
field = input[...input.index("")]
moves = input[input.index("")...].join.chars
orig = (0...field.size).to_a.product((0...field[0].size).to_a).find { |r, c| field[r][c] == "@" }

$dirs = {
  "^" => [-1, 0],
  "v" => [1, 0],
  "<" => [0, -1],
  ">" => [0, 1],
}

class Array
  def deep_clone
    self.map { |e| e.class == Array ? e.clone : e }
  end
end

def find_loc(field, row, col, dir)
  dr, dc = $dirs[dir]

  while field[row + dr][col + dc] == "O"
    row, col = row + dr, col + dc
  end
  row, col = row + dr, col + dc

  field[row][col] == "." ? [row, col] : nil
end

def move_box(field, row, col, dir)
  dr, dc = $dirs[dir]

  while field[row][col] != "@"
    field[row][col] = field[row - dr][col - dc]
    row, col = row - dr, col - dc
  end
  field[row][col] = "."

  [row + dr, col + dc]
end

# Part 1
pos = orig.clone
f = field.deep_clone

moves.each do |move|
  move_pos = find_loc(f, *pos, move)
  if !move_pos.nil?
    pos = move_box(f, *move_pos, move)
  end
end

part1 = (0...f.size).to_a.product((0...f[0].size).to_a)
  .map { |r, c| f[r][c] == "O" ? r * 100 + c : 0 }
  .sum

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = 0

puts "== Part 2 =="
puts part2
