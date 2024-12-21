#!/usr/bin/env ruby

$sample = false
input_path = $sample ? "sample.txt" : "input.txt"

input = File.open(input_path, "r").map(&:strip)
$regs = [
  input[0].split(":")[-1].to_i,
  input[1].split(":")[-1].to_i,
  input[2].split(":")[-1].to_i,
]
$prog = input[4].split()[-1].split(",").map(&:to_i)

def load_opr(op, opr)
  if [0, 2, 5, 6, 7].include?(op)
    [
      -> { opr },
      -> { opr },
      -> { opr },
      -> { opr },
      -> { $regs[0] },
      -> { $regs[1] },
      -> { $regs[2] },
      -> { raise "err: opr `#{opr}` is not valid" },
    ][opr].call
  else
    opr
  end
end

# Part 1
part1 = []

pc = 0
while pc < $prog.size
  op = $prog[pc]
  pc += 1
  opr = load_opr(op, $prog[pc])
  pc += 1

  [
    -> {
      x = $regs[0]
      y = 2**opr
      $regs[0] = x / y
    },
    -> {
      x = $regs[1]
      y = opr
      $regs[1] = x ^ y
    },
    -> {
      x = opr
      $regs[1] = x % 8
    },
    -> {
      if $regs[0] != 0
        pc = opr
      end
    },
    -> {
      x = $regs[1]
      y = $regs[2]
      $regs[1] = x ^ y
    },
    -> {
      x = opr
      part1.push x % 8
    },
    -> {
      x = $regs[0]
      y = 2**opr
      $regs[1] = x / y
    },
    -> {
      x = $regs[0]
      y = 2**opr
      $regs[2] = x / y
    },
  ][op].call
end
part1 = part1.join(",")

puts "== Part 1 =="
puts part1
puts

# Part 2
part2 = 0
prog_str = $prog.join(",")

(0...200000).each do |reg_a|
  $regs = [
    reg_a,
    input[1].split(":")[-1].to_i,
    input[2].split(":")[-1].to_i,
  ]

  out = []

  pc = 0
  while pc < $prog.size
    op = $prog[pc]
    pc += 1
    opr = load_opr(op, $prog[pc])
    pc += 1

    [
      -> {
        x = $regs[0]
        y = 2**opr
        $regs[0] = x / y
      },
      -> {
        x = $regs[1]
        y = opr
        $regs[1] = x ^ y
      },
      -> {
        x = opr
        $regs[1] = x % 8
      },
      -> {
        if $regs[0] != 0
          pc = opr
        end
      },
      -> {
        x = $regs[1]
        y = $regs[2]
        $regs[1] = x ^ y
      },
      -> {
        x = opr
        out.push x % 8
      },
      -> {
        x = $regs[0]
        y = 2**opr
        $regs[1] = x / y
      },
      -> {
        x = $regs[0]
        y = 2**opr
        $regs[2] = x / y
      },
    ][op].call
  end
  out = out.join(",")

  if out == prog_str
    part2 = reg_a
    break
  end
end

puts "== Part 2 =="
puts part2
