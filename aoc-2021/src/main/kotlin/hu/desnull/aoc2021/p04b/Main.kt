package hu.desnull.aoc2021.p04b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

const val N = 5
typealias Board = ArrayList<Pair<Int, Boolean>>

fun isWinner(board: Board, row: Int, col: Int): Boolean {
  var horiz = true
  for (j in 0 until N) {
    horiz = horiz && board[row * N + j].second
  }
  if (horiz) {
    return true
  }
  var vert = true
  for (i in 0 until N) {
    vert = vert && board[i * N + col].second
  }
  if (vert) {
    return true
  }
  return false
}

fun check(board: Board, n: Int): Int {
  var isWinner = false
  for (i in 0 until N) {
    for (j in 0 until N) {
      val idx = i * N + j
      val b = board[idx]
      if (b.first == n && !b.second) {
        board[idx] = b.copy(second = true)
        isWinner = isWinner || isWinner(board, i, j)
      }
    }
  }
  if (!isWinner) {
    return 0
  }
  return n * board.sumOf { if (it.second) 0 else it.first }
}

fun readBoard(): Board? {
  val board = Board()
  for (i in 0 until N * N) {
    val str = readString() ?: return null
    board.add(Pair(str.toInt(), false))
  }
  return board
}

fun main() {
  val draws = readln().split(",").stream().map(String::toInt)
  val boards = generateSequence(::readBoard).toMutableList()
  for (n in draws) {
    val it = boards.iterator()
    while (it.hasNext()) {
      val board = it.next()
      val res = check(board, n)
      if (res != 0) {
        if (boards.size == 1) {
          println(res)
          return
        } else {
          it.remove()
        }
      }
    }
  }
  println(0)
}
