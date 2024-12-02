package hu.desnull.aoc2024.p02b

import kotlin.math.abs

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

fun check(list: List<Int>): Boolean {
  return (
    list.zipWithNext() {
      a, b ->
      a < b
    }.all { it } || list.zipWithNext() {
      a, b ->
      a > b
    }.all { it }
    ) && list.zipWithNext() {
    a, b ->
    abs(a - b) <= 3
  }.all { it }
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
