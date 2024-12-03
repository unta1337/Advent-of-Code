#!/usr/bin/env ruby

input = File.open("input.txt", "r").map { |line| line.strip }

# Part 1
puts "== Part 1 =="
puts input.join.scan(/mul\((\d+),(\d+)\)/).map { |p, q| p.to_i * q.to_i }.sum
puts

# Part 2
puts "== Part 2 =="
part2 = 0
do_add = true

input.join.scan(/(mul\((\d+),(\d+)\)|don't\(\)|do\(\))/) do |op, p, q|
  if op == "do()"
    do_add = true
  elsif op == "don't()"
    do_add = false
  else
    part2 += do_add ? p.to_i * q.to_i : 0
  end
end

puts part2
