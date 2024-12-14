#!/usr/bin/env ruby

input = File.open("input.txt", "r").map(&:strip).each_slice(4)
  .map { |a, b, r, _|
    [a.match(/.*X\+(.+),.*Y\+(.+)/).to_a[1..].map(&:to_i),
     b.match(/.*X\+(.+),.*Y\+(.+)/).to_a[1..].map(&:to_i),
     r.match(/.*X=(.+),.*Y=(.+)/).to_a[1..].map(&:to_i)]
  }

# Part 1
part1 = input.map { |(ax, ay), (bx, by), (rx, ry)|
  # ax * n + bx * m = rx
  # ay * n + by * m = ry
  #
  # 이를 만족하는 (n, m)쌍 중 min(3 * n + m)
  # 존재하지 않으면 0

  n_range = [(rx.to_f / ax).ceil, (ry.to_f / ay).ceil].max
  m_range = [(rx.to_f / bx).ceil, (ry.to_f / by).ceil].max

  ret = nil
  (0..n_range).each do |n|
    (0..m_range).each do |m|
      if ax * n + bx * m == rx && ay * n + by * m == ry
        ret = ret.nil? ? 3 * n + m : [ret, 3 * n + m].min
      end
    end
  end

  ret.nil? ? 0 : ret
}.sum

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = input.map { |(ax, ay), (bx, by), (rx, ry)|
  # ax * n + bx * m = rx
  # ay * n + by * m = ry
  #
  # m = (ax * ry - ay * rx) / (ax * by - ay * bx)
  # n = (rx - bx * m) / ax
  #
  # 이를 만족하는 (n, m)가 존재하면 3 * n + m
  # 존재하지 않으면 0

  rx, ry = rx + 10000000000000, ry + 10000000000000

  m = (ax * ry - ay * rx) / (ax * by - ay * bx)
  n = (rx - bx * m) / ax

  if ax * n + bx * m == rx && ay * n + by * m == ry
    3 * n + m
  else
    0
  end
}.sum

puts "== Part 2 =="
puts part2
