package hu.desnull.aoc2021.p01a

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence { System.`in`.read().toChar() }
  .dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("")

fun main() {
  var last = Integer.MAX_VALUE
  var res = 0
  while (true) {
    val d = readlnOrNull()?.toInt() ?: break
    if (d > last) {
      ++res
    }
    last = d
  }
  println(res)
}
