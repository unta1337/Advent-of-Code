#!/usr/bin/env ruby

$sample = false
input_path = $sample ? "sample.txt" : "input.txt"

input = File.open(input_path, "r").map { |line| line.strip.chars }
rows = input.size
cols = input[0].size
src = (0...rows).to_a.product((0...cols).to_a).find { |r, c| input[r][c] == "S" }
dest = (0...rows).to_a.product((0...cols).to_a).find { |r, c| input[r][c] == "E" }

dirs = [[-1, 0], [0, 1], [1, 0], [0, -1]]

class PriorityQueue
  def initialize &comp
    @data = []
    @cursor = 0
    @comp = if comp.nil? then Proc.new { |p, q| p < q } else comp end
  end

  def empty?
    @cursor == 0
  end

  def push elem
    if @cursor < @data.size then
      @data[@cursor] = elem
    else
      @data.push elem
    end

    reheap_up @cursor
    @cursor += 1
  end

  def pop
    if empty?
      nil
    else
      @cursor -= 1
      @data[0], @data[@cursor] = @data[@cursor], @data[0]
      reheap_down 0

      @data[@cursor]
    end
  end

  def peek
    if empty?
      nil
    else
      @data[0]
    end
  end

  private

  def reheap_up index
    p = (index - 1) / 2

    if p >= 0 && @comp.call(@data[index], @data[p]) then
      @data[index], @data[p] = @data[p], @data[index]
      reheap_up p
    end
  end

  def reheap_down index
    l, r = [index * 2 + 1, index * 2 + 2]

    if l < @cursor then
      less_index = if r < @cursor then
                     if @comp.call(@data[r], @data[l]) then r else l end
                   else
                     l
                   end

      if @comp.call(@data[less_index], @data[index]) then
        @data[less_index], @data[index] = @data[index], @data[less_index]
        reheap_down less_index
      end
    end
  end
end

# Part 1
part1 = Float::INFINITY

q = PriorityQueue.new { |p, q| p[2] <=> q[2] }
lookup = Hash.new

q.push [*src, 1, 0]
lookup[[*src, 1]] = 0

while !q.empty?
  row, col, dir, cost = q.pop
  if !lookup[[row, col, dir]].nil? && lookup[[row, col, dir]] < cost
    next
  end

  if [row, col] == dest
    part1 = [part1, cost].min
    next
  end

  [-1, 0, 1].map { |i| (dir + i) % 4 }.each do |nd|
    if nd == dir
      dr, dc = dirs[nd]
      ns = cost + 1
    else
      dr, dc = 0, 0
      ns = cost + 1000
    end
    nr, nc = row + dr, col + dc

    if input[nr][nc] != "#" && (lookup[[nr, nc, nd]].nil? || ns < lookup[[nr, nc, nd]])
      q.push [nr, nc, nd, ns]
      lookup[[nr, nc, nd]] = ns
    end
  end
end

puts "== Part 1 =="
puts part1
puts

# Part 2
require "set"

part2 = Set.new

q = PriorityQueue.new { |p, q| p[2] <=> q[2] }
lookup = Hash.new

q.push [*src, 1, 0, [src]]
lookup[[*src, 1]] = 0

while !q.empty?
  row, col, dir, cost, path = q.pop
  if !lookup[[row, col, dir]].nil? && lookup[[row, col, dir]] < cost
    next
  end

  path.push [row, col]

  if [row, col] == dest
    if cost == part1
      part2 += path.to_set
    end
    next
  end

  [-1, 0, 1].map { |i| (dir + i) % 4 }.each do |nd|
    if nd == dir
      dr, dc = dirs[nd]
      ns = cost + 1
    else
      dr, dc = 0, 0
      ns = cost + 1000
    end
    nr, nc = row + dr, col + dc

    if input[nr][nc] != "#" && (lookup[[nr, nc, nd]].nil? || ns <= lookup[[nr, nc, nd]])
      q.push [nr, nc, nd, ns, path.clone]
      lookup[[nr, nc, nd]] = ns
    end
  end
end
part2 = part2.size

puts "== Part 2 =="
puts part2
