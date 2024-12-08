#!/usr/bin/env ruby

require "set"

input = File.open("input.txt", "r").map { |row| row.strip.chars }
rows, cols = input.size, input[0].size
origin = (0...rows).to_a.product((0...cols).to_a).find { |i, j| input[i][j] == "^" }
input[origin[0]][origin[1]] = "."

dirs = {
  "^" => [[-1, 0], ">"],
  ">" => [[0, 1], "v"],
  "v" => [[1, 0], "<"],
  "<" => [[0, -1], "^"]
}

# Part 1
part1 = 0
state = [*origin, "^"]
lookup = Hash.new

while !lookup[state]
  lookup[state] = true

  row, col, dir = state
  nr, nc = row + dirs[dir][0][0], col + dirs[dir][0][1]

  if !lookup[[row, col]]
    part1 += 1
    lookup[[row, col]] = true
  end

  if !(nr.between?(0, rows - 1) && nc.between?(0, cols - 1))
    break
  elsif input[nr][nc] == "."
    state = [nr, nc, dir]
  else
    state = [row, col, dirs[dir][1]]
  end
end

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = Set.new
state = [*origin, "^"]
lookup = Hash.new

check_loop = lambda do |row, col, dir|
  check_lookup = Hash.new

  loop do
    if lookup[[row, col, dir]] || check_lookup[[row, col, dir]]
      return true
    end
    check_lookup[[row, col, dir]] = true

    xr, xc = row + dirs[dir][0][0], col + dirs[dir][0][1]
    if !(xr.between?(0, rows - 1) && xc.between?(0, cols - 1))
      return false
    elsif input[xr][xc] == "."
      row, col = xr, xc
    else
      dir = dirs[dir][1]
    end
  end
end

while !lookup[state]
  row, col, dir = state
  nr, nc = row + dirs[dir][0][0], col + dirs[dir][0][1]

  if nr.between?(0, rows - 1) && nc.between?(0, cols - 1) && input[nr][nc] == "." && lookup[[nr, nc]].nil?
    input[nr][nc] = "#"
    if check_loop.call(row, col, dirs[dir][1])
      part2.add [nr, nc]
    end
    input[nr][nc] = "."
  end

  if !lookup[[row, col]]
    lookup[[row, col]] = true
  end
  lookup[state] = true

  if !(nr.between?(0, rows - 1) && nc.between?(0, cols - 1))
    break
  elsif input[nr][nc] == "."
    state = [nr, nc, dir]
  else
    state = [row, col, dirs[dir][1]]
  end
end
part2 = part2.size

puts "== Part 2 =="
puts part2
