#!/usr/bin/env ruby

input = File.open("sample.txt", "r").map { |row| row.strip.chars }
rows, cols = input.size, input[0].size
origin = (0...rows).to_a.product((0...cols).to_a).find { |i, j| input[i][j] == '^' }
input[origin[0]][origin[1]] = '.'

dirs = {
  '^' => [[-1, 0], '>'],
  '>' => [[0, 1], 'v'],
  'v' => [[1, 0], '<'],
  '<' => [[0, -1], '^']
}

# Part 1
part1 = 0
state = [*origin, '^']
lookup = Hash.new

while !lookup[state]
  lookup[state] = true

  row, col, dir = state
  nr, nc = row + dirs[dir][0][0], col + dirs[dir][0][1]

  if !lookup[[row, col]]
    part1 += 1
    lookup[[row, col]] = true
  end

  if input[nr].nil? || input[nr][nc].nil?
    break
  elsif input[nr][nc] == '.'
    state = [nr, nc, dir]
  else
    state = [row, col, dirs[dir][1]]
  end
end

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = 0

puts "== Part 2 =="
puts part2
