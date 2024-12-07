package hu.desnull.aoc2021.p05a

import kotlin.math.sign

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

data class Point(val x: Int, val y: Int)
data class Line(val a: Point, val b: Point)
val lineComparator = compareBy<Point>({ it.x }, { it.y })
fun lineOf(a: Point, b: Point): Line {
  return if (lineComparator.compare(a, b) <= 0) {
    Line(a, b)
  } else {
    Line(b, a)
  }
}

class Board(private val width: Int, private val height: Int) : Iterable<Int> {
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
typealias Lines = List<Line>

fun parsePoint(str: String): Point {
  val xy = str.split(",")
  return Point(xy[0].toInt(), xy[1].toInt())
}

fun parseLine(str: String): Line {
  val ab = str.split(" -> ")
  return Line(parsePoint(ab[0]), parsePoint(ab[1]))
}

fun readLines(): Lines {
  return generateSequence(::readLine)
    .map(::parseLine)
    .filter(::isValid)
    .toList()
}

fun isValid(line: Line): Boolean {
  return line.a.x == line.b.x || line.a.y == line.b.y
}

fun check(board: Board): Int {
  return board.count { it > 1 }
}

fun draw(board: Board, line: Line) {
  val hStep = (line.b.x - line.a.x).sign
  val vStep = (line.b.y - line.a.y).sign
  var i = line.a.x
  var j = line.a.y
  while (i != line.b.x || j != line.b.y) {
    board.inc(i, j)
    i += hStep
    j += vStep
  }
  board.inc(i, j)
}

fun main() {
  val lines = readLines()
  var width = 0
  var height = 0
  for (line in lines) {
    if (line.a.x > width) {
      width = line.a.x
    }
    if (line.b.x > width) {
      width = line.b.x
    }
    if (line.a.y > height) {
      height = line.a.y
    }
    if (line.b.y > height) {
      height = line.b.y
    }
  }
//  println("width: $width, height: $height")
  val board = Board(width + 1, height + 1)
  for (line in lines) {
    draw(board, line)
  }
  println(check(board))
}
