package hu.desnull.aoc2021.p01b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence { System.`in`.read().toChar() }
  .dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("")

fun main() {
  var last = Integer.MAX_VALUE
  var c = 0
  val b = ArrayDeque<Int>()
  var res = 0
  while (true) {
    val d = readlnOrNull()?.toInt() ?: break
    b.addLast(d)
    c += d
    if (b.size < 3) {
      continue
    }
    if (b.size > 3) {
      c -= b.removeFirst()
    }
    if (c > last) {
      ++res
    }
    last = c
  }
  println(res)
}
