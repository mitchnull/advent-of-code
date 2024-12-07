package hu.desnull.aoc2022.p02a

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

private fun readLines() = generateSequence {
  readlnOrNull()
}

private fun v(c: Char): Int {
  return when (c) {
    'A' -> 1
    'B' -> 2
    'C' -> 3
    'X' -> 1
    'Y' -> 2
    'Z' -> 3
    else -> 0
  }
}

private fun score(a: Char, x: Char): Int {
  val va = v(a)
  val vx = v(x)
  return vx + ((vx - va + 4) % 3) * 3
}

private fun score(ax: List<String>): Int {
  return score(ax.get(0)[0], ax.get(1)[0])
}

fun main() {
  val res = readLines().map {
    it.split(" ")
  }.map(::score).sum()
  println(res)
}
