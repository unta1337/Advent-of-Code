#!/usr/bin/env ruby

$sample = false
input_path = $sample ? "sample.txt" : "input.txt"

input = File.open(input_path, "r").map(&:strip)
available_patterns = input[0].split(",").map(&:strip)
need_to_make = input[2..]

# Part 1
expr = Regexp.new "^(" + available_patterns.join("|") + ")*$"
part1 = need_to_make.map { |design| design.match?(expr) ? 1 : 0 }.sum

puts "== Part 1 =="
puts part1
puts

# Part 2
def match_str(target, strs, cache = nil)
  if cache.nil?
    cache = Hash.new
  end

  if target == ""
    return 1
  elsif !cache[target].nil?
    return cache[target]
  end

  ret = 0
  strs.each do |str|
    if target[...str.size] == str
      ret += match_str target[str.size...], strs, cache
    end
  end

  cache[target] = ret
end

part2 = need_to_make.map { |design| match_str(design, available_patterns) }.sum

puts "== Part 2 =="
puts part2
