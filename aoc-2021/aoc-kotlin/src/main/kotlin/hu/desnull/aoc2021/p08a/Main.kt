package hu.desnull.aoc2021.p08a

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

private fun readLines() = generateSequence {
  readlnOrNull()
}

const val N = 8

fun main() {
  val counts = IntArray(N) { 0 }
  readLines().map { it.split("|")[1] }
    .flatMap { it.split(" ") }
    .map(String::trim)
    .map(String::length)
    .forEach { ++counts[it] }
  val uniques = arrayOf(2, 3, 4, 7).sumOf { counts[it] }
  println(uniques)
}
