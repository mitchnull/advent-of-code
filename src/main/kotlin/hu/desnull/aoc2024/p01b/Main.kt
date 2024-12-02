package hu.desnull.aoc2024.p01b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

fun main() {
  val left = ArrayList<Int>()
  val right = ArrayList<Int>()
  readLines().forEach {
    val (l, r) = it.split("\\s+".toRegex(), 2)
    left.add(l.toInt())
    right.add(r.toInt())
  }
  val sum = left.stream().map { it * right.count { r -> r == it } }.reduce(Integer::sum).orElse(0)
  println(sum)
}
