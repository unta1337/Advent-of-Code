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
part2 = input.clone
75.times do |i|
  puts "Part 2: #{i + 1}/#{75}"

  part2.map! { |stone|
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
part2 = part2.size

puts "== Part 2 =="
puts part2
