#!/usr/bin/env ruby

input = File.open("input.txt", "r").map { |line| line.split.map(&:to_i) }

# Part 1
puts "== Part 1 =="
puts input.map { |data|
  diff = data.each_cons(2).map { |p, q| p - q }
  sign = diff.all? { |d| d >= 0 } || diff.all? { |d| d <= 0 }
  rng = diff.all? { |d| d.abs.between?(1, 3) }

  sign && rng ? 1 : 0
}.sum
puts

# Part 2
puts "== Part 2 =="
puts input.map { |data|
  (0..data.size).map { |i|
    dt = data.reject.with_index { |_, j| j == i }
    diff = dt.each_cons(2).map { |p, q| p - q }
    sign = diff.all? { |d| d >= 0 } || diff.all? { |d| d <= 0 }
    rng = diff.all? { |d| d.abs.between?(1, 3) }

    sign && rng ? 1 : 0
  }.any? { |r| r == 1 } ? 1 : 0
}.sum
