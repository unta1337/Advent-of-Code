#!/usr/bin/env ruby

require "set"

$sample = false
input_path = $sample ? "sample.txt" : "input.txt"

input = File.open(input_path, "r").map { |line| line.split(",").map(&:to_i) }
corrupted = $sample ? input[...12].to_set : input[...1024].to_set
dest = $sample ? [6, 6] : [70, 70]

# Part 1
puts "== Part 1 =="
q = Queue.new
visited = Set.new

q.push [0, 0]
visited.add [0, 0]
steps = 0

while !q.empty?
  iter = q.size
  iter.times do
    x, y = q.pop
    if [x, y] == dest
      puts steps
      next
    end

    [[-1, 0], [1, 0], [0, -1], [0, 1]].each do |dx, dy|
      nx, ny = x + dx, y + dy
      if nx.between?(0, dest[0]) && ny.between?(0, dest[1]) && !corrupted.include?([nx, ny]) && !visited.include?([nx, ny])
        q.push [nx, ny]
        visited.add [nx, ny]
      end
    end
  end

  steps += 1
end
puts

# Part 2
def can_reach(dest, corrupted)
  q = Queue.new
  visited = Set.new

  q.push [0, 0]
  visited.add [0, 0]

  while !q.empty?
    iter = q.size
    iter.times do
      x, y = q.pop
      if [x, y] == dest
        return true
      end

      [[-1, 0], [1, 0], [0, -1], [0, 1]].each do |dx, dy|
        nx, ny = x + dx, y + dy
        if nx.between?(0, dest[0]) && ny.between?(0, dest[1]) && !corrupted.include?([nx, ny]) && !visited.include?([nx, ny])
          q.push [nx, ny]
          visited.add [nx, ny]
        end
      end
    end
  end

  return false
end

puts "== Part 2 =="
(($sample ? 12 : 1024)...input.size).each do |i|
  c = input[i]
  corrupted.add c

  if !can_reach(dest, corrupted)
    puts c.join(",")
    break
  end
end
