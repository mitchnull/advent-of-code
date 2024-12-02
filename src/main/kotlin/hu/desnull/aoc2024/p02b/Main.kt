package hu.desnull.aoc2024.p02b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

fun check(list: List<Int>): Boolean {
  val diffs = list.zipWithNext() {
    a, b ->
    a - b
  }
  return diffs.all {
    when (it) { in (1..3) -> true else -> false }
  } ||
    diffs.all {
      when (it) { in (-3..-1) -> true else -> false }
    }
}

fun main() {
  val res = readLines()
    .map { it.split("\\s+".toRegex()).map(String::toInt) }
    .map {
      (0..it.size).map { d ->
        check(it.filterIndexed { i, _ -> i != d })
      }.any { it }
    }.count { it }
  println(res)
}
