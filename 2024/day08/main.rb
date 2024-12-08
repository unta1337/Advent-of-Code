#!/usr/bin/env ruby

require "set"

input = File.open("input.txt", "r").map { |row| row.strip.chars }
rows = input.size
cols = input[0].size

antennas = (0...rows).to_a.product((0...cols).to_a)
  .filter { |i, j| input[i][j] != '.' }
  .map { |i, j| [input[i][j], [i, j]] }
  .group_by { |symbol, loc| symbol }
antennas.each do |symbol, locs|
  antennas[symbol] = locs.map { |loc| loc[1] }
end

# Part 1
part1 = antennas.map { |symbol, locs|
  locs.combination(2).map { |(x1, y1), (x2, y2)|
    dx = x2 - x1
    dy = y2 - y1
    [[x1 - dx, y1 - dy], [x2 + dx, y2 + dy]]
      .filter { |x, y| x.between?(0, cols - 1) && y.between?(0, rows - 1) }
  }.reduce([], :+)
}.reduce([], :+)
  .to_set.size

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = antennas.map { |symbol, locs|
  locs.combination(2).map { |(x1, y1), (x2, y2)|
    res = []
    dx = x2 - x1
    dy = y2 - y1

    (0..).each do |i|
      x = x1 - i * dx
      y = y1 - i * dy
      if !(x.between?(0, cols - 1) && y.between?(0, rows - 1))
        break
      end

      res.push [x, y]
    end

    (0..).each do |i|
      x = x2 + i * dx
      y = y2 + i * dy
      if !(x.between?(0, cols - 1) && y.between?(0, rows - 1))
        break
      end

      res.push [x, y]
    end

    res
  }.reduce([], :+)
}.reduce([], :+)
  .to_set.size

puts "== Part 2 =="
puts part2
