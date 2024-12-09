package hu.desnull.aoc2024.p09

data class Block(val id: Int, val len: Int)

fun main() {
  val line = readln()

  val res1 = solve1(line)
  println("1: $res1")

  val res2 = solve2(line)
  println("2: $res2")
}

fun solve1(line: String) : Long {
  val disk = ArrayList<Int>()
  for ((i, v) in line.withIndex()) {
    for (j in 1..v - '0') {
      disk.add(if (i % 2 == 0) i / 2 else -1)
    }
  }
  var b = 0
  var e = disk.size - 1
  while (b < e) {
    if (disk[b] >= 0) {
      ++b
    } else if (disk[e] < 0) {
      --e
    } else {
      disk[b++] = disk[e--]
    }
  }
  var res = 0L
  for (i in 0 until b) {
    res += i * disk[i]
  }
  return res
}

fun solve2(line: String): Long {
  val blocks = ArrayList<Block>()
  for ((i, v) in line.withIndex()) {
    blocks.add(Block(if (i % 2 == 0) i / 2 else -1, v - '0'))
  }
  var e = blocks.size - 1
  blocks.add(Block(-1, 0))
  while (e > 1) {
    val block = blocks[e]
    if (block.id < 0) {
      --e
      continue
    }
    val pos = findFreePos(blocks, e, block.len)
    if (pos < e) {
      blocks[e] = Block(-1, block.len)
      val free = blocks.removeAt(pos)
      if (free.len > block.len) {
        blocks.add(pos, Block(-1, free.len - block.len))
      }
      blocks.add(pos, block)
    }
    --e
  }

  var pos = 0
  var res = 0L
  for (b in blocks) {
    val id = if (b.id > 0) b.id.toLong() else 0
    res += id * (b.len * (2 * pos + b.len - 1) / 2)
    pos += b.len
  }
  return res
}

fun findFreePos(blocks: ArrayList<Block>, e: Int, len: Int): Int {
  for (i in 1 until e ) {
    if (blocks[i].id < 0 && blocks[i].len >= len) {
      return i
    }
  }
  return e
}
