package hu.desnull.aoc2022.p01b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

private fun readLines() = generateSequence {
  readlnOrNull()
}

private fun readElf() = generateSequence {
  readLines()
    .takeWhile(String::isNotEmpty)
    .map(String::toInt).toList().ifEmpty { null }?.sum()
}

fun main() {
  println(readElf().toList().sortedDescending().take(3).sum())
}
