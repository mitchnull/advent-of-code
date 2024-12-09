package hu.desnull.aoc2024.p09

data class Block(val id: Int, val len: Int)

fun main() {
  val line = readln()

  val res1 = solve1(line.toCharArray())
  println("1: $res1")

  val res2 = solve2(line.toCharArray())
  println("2: $res2")
}

fun solve1(line: CharArray): Long {
  val compressed = ArrayList<Int>()
  var b = 0
  var e = ((line.size - 1)/ 2) * 2
  while (b <= e) {
    val id = b / 2
    var len = line[b++] - '0'
    while (len-- > 0) {
      compressed.add(id)
    }
    if (b >= e) {
      break
    }
    var free = line[b++] - '0'
    while (free > 0 && b < e) {
      val id2 = e / 2
      var len2 = line[e] - '0'
      while (len2 > 0 && free > 0) {
        --len2
        --free
        compressed.add(id2)
      }
      if (len2 > 0) {
        line[e] = '0' + len2
      } else {
        e -= 2
      }
    }
  }
  var res : Long = 0
  for ((i, v) in compressed.withIndex()) {
    res += i * v
  }
  return res
}

fun solve2(line: CharArray): Long {
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
