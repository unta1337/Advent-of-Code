#!/usr/bin/env ruby

require "set"

input = File.open("input.txt", "r").map { |row| row.strip.chars.map(&:to_i) }[0]

# Part 1
data = []
id = 0

input.each_with_index do |count, i|
  if i % 2 == 0
    data.append Array.new(count, id)
    id += 1
  else
    data.append Array.new
  end
end

push_index = 1
pop_index = input.size - 1

while push_index < pop_index
  if data[push_index].size < input[push_index]
    while data[pop_index].size == 0
      pop_index -= 2
    end
    data[push_index].append data[pop_index].pop
  else
    push_index += 2
  end
end
data.flatten!

part1 = data.map.with_index { |d, i| d * i }.sum

puts "== Part 1 =="
puts part1
puts

# Part 2
data = []
id = 0

input.each_with_index do |count, i|
  if i % 2 == 0
    data.append Array.new(count, id)
    id += 1
  else
    data.append Array.new
  end
end

(0...input.size).reverse_each do |pop_index|
  if data[pop_index].size == 0
    next
  end

  push_index = (0...pop_index).find { |i| input[i] - data[i].size >= data[pop_index].size }
  if !push_index.nil?
    data[push_index] += data[pop_index]
    data[pop_index] = []
  end
end
data = data.map.with_index { |d, i| d + Array.new(input[i] - d.size, 0) }.flatten

part2 = data.map.with_index { |d, i| d * i }.sum

puts "== Part 2 =="
puts part2
