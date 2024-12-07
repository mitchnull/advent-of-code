package hu.desnull.aoc2022.p01a

import java.lang.Integer.max

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

fun main() {
  val lines = readLines().toList()
  var most = 0
  var curr = 0
  for (line in lines) {
    if (line.isEmpty()) {
      most = max(curr, most)
      curr = 0
      continue
    }
    curr += line.toInt()
  }
  most = max(curr, most)
  println(most)
}
