#!/usr/bin/env ruby

input = File.open("input.txt", "r").to_a[0].split.map(&:to_i)

# Part 1
part1 = input.clone
25.times do
  part1.map! { |stone|
    if stone == 0
      [1]
    elsif stone.to_s.size % 2 == 0
      s = stone.to_s
      [s[...s.size / 2].to_i, s[s.size / 2...].to_i]
    else
      [stone * 2024]
    end
  }.flatten!
end
part1 = part1.size

puts "== Part 1 =="
puts part1
puts

# Part 2
def calc_part2 i, depth, cache = Hash.new
  if cache.include?([i, depth])
    cache[[i, depth]]
  else
    cache[[i, depth]] = if depth == 0
                 1
               elsif i == 0
                 calc_part2 1, depth - 1, cache
               elsif i.to_s.size % 2 == 0
                 s = i.to_s
                 calc_part2(s[...s.size / 2].to_i, depth - 1, cache) +
                   calc_part2(s[s.size / 2...].to_i, depth - 1, cache)
               else
                 calc_part2 i * 2024, depth - 1, cache
               end
  end
end

part2 = input.map.with_index { |i, j| calc_part2 i, 75 }.sum

puts "== Part 2 =="
puts part2
