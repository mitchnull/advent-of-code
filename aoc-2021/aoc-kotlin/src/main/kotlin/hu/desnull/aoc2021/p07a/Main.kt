package hu.desnull.aoc2021.p07a

import java.lang.Integer.max
import java.lang.Integer.min
import kotlin.math.abs
import kotlin.streams.toList

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

typealias Num = Int
typealias Positions = List<Num>

fun calc(positions: Positions, i: Num): Num {
  return positions.sumOf {
    abs(i - it)
  }
}

fun main() {
  val positions = readln().split(",").stream().map(String::toInt).toList()
  var min = Num.MAX_VALUE
  var max = Num.MIN_VALUE
  for (p in positions) {
    min = min(min, p)
    max = max(max, p)
  }
  var currMin = Num.MAX_VALUE
  for (i in min..max) {
    val fuel = calc(positions, i)
    currMin = min(currMin, fuel)
  }
  println(currMin)
}
