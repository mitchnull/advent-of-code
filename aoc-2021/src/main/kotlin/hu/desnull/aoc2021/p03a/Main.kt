package hu.desnull.aoc2021.p03a

import kotlin.system.exitProcess

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence { System.`in`.read().toChar() }
  .dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("")

fun main() {
  var ones = Array(0) { 0 }
  var zeros = Array(0) { 0 }
  while (true) {
    val line = readlnOrNull() ?: break
    ones = ones.ifEmpty { Array(line.length) { 0 } }
    zeros = zeros.ifEmpty { Array(line.length) { 0 } }
    for (i in line.indices) {
      when (line[i]) {
        '0' -> {
          ++zeros[i]
        }
        '1' -> {
          ++ones[i]
        }
        else -> {
          println("ERROR: $line")
          exitProcess(-1)
        }
      }
    }
  }
  var g = 0
  var e = 0
  for (i in ones.indices) {
    g = g shl 1
    e = e shl 1
    if (ones[i] > zeros[i]) {
      g = g or 1
    } else {
      e = e or 1
    }
  }
  println(g * e)
}
