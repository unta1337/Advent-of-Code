#!/usr/bin/env ruby

$sample = false

input_file = $sample ? "sample.txt" : "input.txt"
input = File.open(input_file, "r").map(&:strip)
  .map { |line|
    line.match(/p=(-?\d+),(-?\d+).*v=(-?\d+),(-?\d+)/)[1..].map(&:to_i)
  }

# Part 1
part1 = input.map { |x, y, dx, dy|
  if $sample
    [(x + dx * 100) % 11 - 5,
     (y + dy * 100) % 7 - 3]
  else
    [(x + dx * 100) % 101 - 50,
     (y + dy * 100) % 103 - 51]
  end
}.group_by { |x, y|
  if x > 0 && y > 0
    1
  elsif x < 0 && y > 0
    2
  elsif x < 0 && y < 0
    3
  elsif x > 0 && y < 0
    4
  else
    0
  end
}
part1.each { |x, xs| part1[x] = xs.size }
part1 = part1.filter { |x, xs| x != 0 }.map { |x, xs| xs }.reduce(:*)

puts "== Part 1 =="
puts part1
puts

# Part 2
puts "== Part 2 =="
(0...).each do |t|
  if $sample
    canvas = Array.new(7) { Array.new(11, ".") }
  else
    canvas = Array.new(103) { Array.new(101, ".") }
  end

  input.each do |(x, y, dx, dy)|
    if $sample
      canvas[(y + t * dy) % 7][(x + t * dx) % 11] = "#"
    else
      canvas[(y + t * dy) % 103][(x + t * dx) % 101] = "#"
    end
  end

  if canvas.any? { |row| row.join.include?("##########") }
    puts canvas.map(&:join).join("\n")
    puts t
    break
  end
end
