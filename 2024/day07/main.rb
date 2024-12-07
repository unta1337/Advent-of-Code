#!/usr/bin/env ruby

input = File.open("input.txt", "r")
  .map { |row| row.split.map(&:to_i) }
  .map { |row| [row[0], row[1..]] }

def eval_expr_part1(oprs, ops)
  res = oprs[0]
  oprs[1..].each do |opr|
    if ops % 2 == 0
      res = res + opr
    else
      res = res * opr
    end

    ops >>= 1
  end
  raise "operators not all used" unless ops == 0

  res
end

def eval_expr_part2(oprs, ops)
  res = oprs[0]
  oprs[1..].each do |opr|
    if ops % 3 == 0
      res = res + opr
    elsif ops % 3 == 1
      res = res * opr
    else
      res = (res.to_s + opr.to_s).to_i
    end

    ops /= 3
  end
  raise "operators not all used" unless ops == 0

  res
end

# Part 1
part1 = input.filter { |(res, oprs)|
  (0...1 << (oprs.size - 1)).any? { |ops| res == eval_expr_part1(oprs, ops) }
}.map { |(res, oprs)| res }.sum

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = input.filter { |(res, oprs)|
  (0...3**(oprs.size - 1)).any? { |ops| res == eval_expr_part2(oprs, ops) }
}.map { |(res, oprs)| res }.sum

puts "== Part 2 =="
puts part2
