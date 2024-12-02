#!/usr/bin/env ruby

input = File.open("input.txt", "r").map { |line| line.split.map(&:to_i) }

# Part 1
puts "== Part 1 =="
puts input
  .transpose.map(&:sort!)
  .transpose.map { |p, q| (p - q).abs }
  .reduce(:+)
puts

# Part 2
puts "== Part 2 =="
xs, ys = input.transpose
puts xs.map { |x| x * ys.count(x) }.reduce(:+)
