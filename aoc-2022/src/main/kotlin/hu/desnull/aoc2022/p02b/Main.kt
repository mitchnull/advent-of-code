package hu.desnull.aoc2022.p02b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

private fun readLines() = generateSequence {
  readlnOrNull()
}

private fun v(c: Char): Int {
  return c - 'A' + 1
}

private fun score(a: Char, x: Char): Int {
  val va = v(a)
  val vx = v(x)
  return vx + ((vx - va + 4) % 3) * 3
}

private fun guess(a: Char, x: Char): Char {
  return 'A' + ((a - 'A') + (x - 'Y') + 3) % 3
}

private fun guessAndScore(a: Char, x: Char): Int {
  return score(a, guess(a, x))
}

private fun guessAndScore(ax: List<String>): Int {
  return guessAndScore(ax.get(0)[0], ax.get(1)[0])
}

fun main() {
  val res = readLines().map {
    it.split(" ")
  }.map(::guessAndScore).sum()
  println(res)
}
