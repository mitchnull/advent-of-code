package hu.desnull.aoc2024.p03b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

fun main() {
  val text = String(System.`in`.readAllBytes(), Charsets.UTF_8)
    .replace("\\s+".toRegex(), " ")
    .replace("don't[(][)].*?(do[(][)]|$)".toRegex(), "")
  val regex = Regex("mul[(]([0-9]{1,3}),([0-9]{1,3})[)]")
  val res = regex.findAll(text).map {
    it.groupValues[1].toInt() * it.groupValues[2].toInt()
  }.sum()
  println(res)
}
