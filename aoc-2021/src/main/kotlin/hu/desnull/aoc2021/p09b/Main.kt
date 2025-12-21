package hu.desnull.aoc2021.p09b

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
const val BASIN = Int.MAX_VALUE - 1
const val PLATEAU = 9

fun print(board: Board) {
  for (y in 0 until board.height) {
    for (x in 0 until board.width) {
      if (board[x, y] == BASIN) {
        print('*')
      } else {
        print(board[x, y])
      }
    }
    println()
  }
}

fun get(board: Board, x: Int, y: Int) : Int {
  if (x < 0 || x >= board.width || y < 0 || y >= board.height) {
    return OFF_BOARD
  }
  return board[x, y]
}

data class Point(val x: Int, val y: Int) {}

fun nextBasinSize(board: Board) : Int? {
  for (y in 0 until board.height) {
    for (x in 0 until board.width) {
      if (board[x, y] < PLATEAU) {
        return basinSize(board, x, y)
      }
    }
  }
  return null
}

fun basinSize(board: Board, x: Int, y: Int): Int? {
  val queue = ArrayList<Point>()
  board[x, y] = BASIN;
  queue.add(Point(x, y))
  var res = 1
  while (queue.isNotEmpty()) {
    val (x, y) = queue.removeLast()
    res += checkBasin(board, x - 1, y, queue)
    res += checkBasin(board, x + 1, y, queue)
    res += checkBasin(board, x, y - 1, queue)
    res += checkBasin(board, x, y + 1, queue)
  }
//  print(board); println(res)
  return res
}

fun checkBasin(board: Board, x: Int, y: Int, queue: ArrayList<Point>): Byte {
  if (get(board, x, y) < PLATEAU) {
    board[x, y] = BASIN
    queue.add(Point(x, y))
    return 1
  }
  return 0
}

fun check(board: Board): Int {
  return generateSequence {
    nextBasinSize(board)
  }.sortedDescending()
    .take(3)
    .fold(1) {
    acc, v -> acc * v
  }
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
