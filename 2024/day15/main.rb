#!/usr/bin/env ruby

$sample = false
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

def get_push_loc(field, row, col, dir, visited = nil)
  if visited.nil?
    visited = Array.new(field.size) { Array.new(field[0].size, false) }
  end

  visited[row][col] = true

  ret = [[row, col]]
  ddr, ddc = $dirs[dir]

  [[-1, 0], [1, 0], [0, -1], [0, 1]].each do |dr, dc|
    if [ddr, ddc] == [-dr, -dc] || ddc != 0 && dr != 0
      next
    end

    nr, nc = row + dr, col + dc
    if visited[nr][nc]
      next
    end

    if ddc != 0
      if field[nr][nc] == "#"
        ret = []
      elsif dc != 0 && "[]".include?(field[nr][nc])
        res = get_push_loc(field, nr, nc, dir, visited)
        ret = res.size > 0 && ret.size > 0 ? ret + res : []
      end
    else
      if dr != 0
        if "[]".include?(field[nr][nc])
          res = get_push_loc(field, nr, nc, dir, visited)
          ret = res.size > 0 && ret.size > 0 ? ret + res : []
        elsif field[nr][nc] != "."
          ret = []
        end
      else
        pair = dc < 0 ? field[nr][nc] + field[row][col] : field[row][col] + field[nr][nc]
        if pair == "[]"
          res = get_push_loc(field, nr, nc, dir, visited)
          ret = res.size > 0 && ret.size > 0 ? ret + res : []
        end
      end
    end
  end

  ret
end

def push_box(field, locs, dir)
  bak = Hash.new
  locs.each do |r, c|
    bak[[r, c]] = field[r][c]
    field[r][c] = "."
  end

  dr, dc = $dirs[dir]
  locs.each do |r, c|
    field[r + dr][c + dc] = bak[[r, c]]
  end

  [locs[0][0] + dr, locs[0][1] + dc]
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

moves.each do |move|
  locs = get_push_loc(f, *pos, move)
  if locs.size > 0
    pos = push_box(f, locs, move)
  end
end

part2 = (0...f.size).to_a.product((0...f[0].size).to_a)
  .map { |r, c| f[r][c] == "[" ? r * 100 + c : 0 }
  .sum

puts "== Part 2 =="
puts part2
