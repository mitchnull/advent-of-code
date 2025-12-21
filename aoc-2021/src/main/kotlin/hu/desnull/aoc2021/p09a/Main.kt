package hu.desnull.aoc2021.p09a

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

class Board(val width: Int, val height: Int) : Iterable<Int> {
  private val items = Array(width * height) { 0 }

  private fun idx(x: Int, y: Int): Int {
    return y * width + x
  }

  operator fun get(x: Int, y: Int): Int {
    return items[idx(x, y)]
  }

  operator fun set(x: Int, y: Int, v: Int) {
    items[idx(x, y)] = v
  }

  fun inc(x: Int, y: Int): Int {
    return ++items[idx(x, y)]
  }

  override fun iterator() = items.iterator()
}

const val OFF_BOARD = Int.MAX_VALUE

fun get(board: Board, x: Int, y: Int) : Int {
  if (x < 0 || x >= board.width || y < 0 || y >= board.height) {
    return OFF_BOARD
  }
  return board[x, y]
}

fun riskLevel(board: Board, x: Int, y: Int) : Int {
  val level = board[x, y]
  if (get(board, x - 1, y) <= level
    || get(board, x + 1, y) <= level
    || get(board, x, y - 1) <= level
    || get(board, x, y + 1) <= level) {
    return 0
  }
  return level + 1
}

fun check(board: Board): Int {
  var res = 0
  for (y in 0 until board.height) {
    for (x in 0 until board.width) {
      res += riskLevel(board, x, y)
    }
  }
  return res
}

fun readLines() = generateSequence {
  readlnOrNull()
}

fun main() {
  val lines = readLines().toList()
  val board = Board(lines.first().length, lines.size)
  for (y in 0 until board.height) {
    for (x in 0 until board.width) {
      board[x, y] = lines[y][x] - '0'
    }
  }
  println(check(board))
}
