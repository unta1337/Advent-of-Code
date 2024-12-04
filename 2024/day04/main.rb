#!/usr/bin/env ruby

input = File.open("input.txt", "r").map(&:strip)
rows = input.size
cols = input[0].size

# Part 1
puts "== Part 1 =="

row_wise = input.map { |row| row.scan("XMAS").count + row.reverse.scan("XMAS").count }.sum
col_wise = input.map{ |row| row.chars }.transpose.map { |col| col.join }
  .map { |col| col.scan("XMAS").count + col.reverse.scan("XMAS").count }.sum
diag = (0..rows + cols - 2).map { |s|
  d1 = (0...rows).to_a.product((0...cols).to_a).filter { |i, j| i + j == s }
    .map { |i, j| input[i][j] }.join
  d2 = (0...rows).to_a.product((0...cols).to_a).filter { |i, j| i + (cols - 1 - j) == s }
    .map { |i, j| input[i][j] }.join

  d1.scan("XMAS").count + d1.reverse.scan("XMAS").count +
  d2.scan("XMAS").count + d2.reverse.scan("XMAS").count
}.sum

count = row_wise + col_wise + diag
puts count

puts

# Part 2
puts "== Part 2 =="
puts input.each_cons(3).map { |row|
  xs = row[0].chars.each_cons(3).to_a.zip(row[1].chars.each_cons(3).to_a,
                                          row[2].chars.each_cons(3).to_a)
  xs.map { |con| con.map{ |con| con.join } }
    .map { |con|
      x = (con[0].match(/M.S/).nil? ? 0 : 1) +
          (con[1].match(/.A./).nil? ? 0 : 1) +
          (con[2].match(/M.S/).nil? ? 0 : 1) == 3 ? 1 : 0
      y = (con[0].match(/S.S/).nil? ? 0 : 1) +
          (con[1].match(/.A./).nil? ? 0 : 1) +
          (con[2].match(/M.M/).nil? ? 0 : 1) == 3 ? 1 : 0
      z = (con[0].match(/M.M/).nil? ? 0 : 1) +
          (con[1].match(/.A./).nil? ? 0 : 1) +
          (con[2].match(/S.S/).nil? ? 0 : 1) == 3 ? 1 : 0
      w = (con[0].match(/S.M/).nil? ? 0 : 1) +
          (con[1].match(/.A./).nil? ? 0 : 1) +
          (con[2].match(/S.M/).nil? ? 0 : 1) == 3 ? 1 : 0

      x + y + z + w
    }.sum
}.sum
