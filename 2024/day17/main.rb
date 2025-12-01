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
#
# |Combo Operands|     |Operators|
#  0-3: Literal         0 adv: A = A / 2^COM
#  4: reg A             1 bxl: B = B ^ LIT
#  5: reg B             2 bst: B = COM % 8
#  6: reg C             3 jnz: if A != 0 { jmp LIT }
#  7: None              4 bxc: B = B ^ C
#                       5 out: puts (COM % 8)
#                       6 bdv: B = A / 2^COM
#                       7 cdv: C = A / 2^COM
# 
# Program: 2,4,1,5,7,5,1,6,4,2,5,5,0,3,3,0
# Program: bst 4      ; B = A & 0b111
#          bxl 5      ; B = B ^ 0b101
#          cdv 5      ; C = A >> B
#          bxl 6      ; B = B ^ 0b110
#          bxc 2      ; B = B ^ C
#          out 5      ; puts (B & 0b111)
#          adv 3      ; A >> 3
#          jnz 0

part2 = 0
prog_str = $prog.join(",")

def run_program(input, reg_a)
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
  out
end

def run_program_optimized(reg_a)
  x = reg_a
  out = []
  while true
    d = ((((x & 0b111) ^ 0b101) ^ 0b110) ^ (x >> ((x & 0b111) ^ 0b101))) & 0b111
    out.append d
    x = x / 0b1000

    if x == 0
      break
    end
  end
  out
end

def search(chunk, chunk_count, target, target_index)
  if target_index < 0
    return chunk
  end

  ret = nil
  (0b000..0b111).each do |c|
    next_chunk = (chunk << 3) | c
    prog_res = run_program_optimized(next_chunk)
    if prog_res[0] == target[target_index]
      res = search(next_chunk, chunk_count + 1, target, target_index - 1)
      if !res.nil?
        return res
      end
    end
  end

  ret
end

part2 = search(0, 0, $prog, $prog.size - 1)

puts "== Part 2 =="
puts part2
