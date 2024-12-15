#!/usr/bin/env ruby

$sample = true
input_path = $sample ? "sample.txt" : "input.txt"

input = File.open(input_path, "r").map(&:strip)
field = input[...input.index("")]
moves = input[input.index("")...].join.chars

$dirs = {
  "^" => [-1, 0],
  "v" => [1, 0],
  "<" => [0, -1],
  ">" => [0, 1],
}

class Array
  def deep_clone
    self.map { |e| e.class == Array || e.class == String ? e.clone : e }
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
f = field.deep_clone
pos = (0...f.size).to_a.product((0...f[0].size).to_a).find { |r, c| f[r][c] == "@" }

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
f = field.map { |row| row.chars.map { |cell| cell == "O" ? "[]" : cell == "@" ? "@." : cell * 2 }.join }
pos = (0...f.size).to_a.product((0...f[0].size).to_a).find { |r, c| f[r][c] == "@" }

puts f.join("\n")
puts "#{pos}"

part2 = 0

puts "== Part 2 =="
puts part2
