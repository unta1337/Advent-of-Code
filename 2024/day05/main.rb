#!/usr/bin/env ruby

class Array
  def split delim
    ret = []
    prev = -1
    indexes = [-1] + (0...self.size).filter { |i| self[i] == delim } + [self.size]
    indexes.each_cons(2).map { |p, i| self[p + 1...i] }
  end
end

input = File.open("input.txt", "r").map(&:split)
order, sorted = input.split []
order = order
  .map { |o| o[0].split("|").map(&:to_i) }
  .group_by { |p, q| p }
order.each { |p, qs| order[p] = qs.map { |q| q[1] } }
sorted = sorted.map { |s| s[0].split(",").map(&:to_i) }

# Part 1
part1 = sorted.filter { |s|
  s.each_cons(2).find { |(p, q)| order[p].nil? || !order[p].include?(q) }.nil?
}.map { |s| s[s.size / 2] }.sum

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = sorted.filter { |s|
  !s.each_cons(2).find { |(p, q)| order[p].nil? || !order[p].include?(q) }.nil?
}.map { |s|
  s.sort { |p, q|
    !order[p].nil? && order[p].include?(q) ? -1 : 1
  }[s.size / 2]
}.sum

puts "== Part 2 =="
puts part2
