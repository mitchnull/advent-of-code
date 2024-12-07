package hu.desnull.aoc2021.p03b

import java.util.stream.Collectors
import kotlin.system.exitProcess

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence { System.`in`.read().toChar() }
  .dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("")

typealias V = List<String>

enum class Commonality {
  MostCommon,
  LeastCommon,
}

tailrec fun getValue(v: V, p: Int, comm: Commonality): Int {
  var zeros = 0
  var ones = 0
  for (s in v) {
    when (s[p]) {
      '0' -> ++zeros
      '1' -> ++ones
      else -> {
        println("ERROR: $s")
        exitProcess(-1)
      }
    }
  }
  val sel = when (comm) {
    Commonality.MostCommon -> if (ones >= zeros) '1' else '0'
    Commonality.LeastCommon -> if (ones < zeros) '1' else '0'
  }
  val vv = v.stream().filter { it[p] == sel }.collect(Collectors.toList())
  if (vv.isEmpty()) {
    println("ERROR: empty")
    exitProcess(-1)
  }
  if (vv.size == 1) {
    return vv.first().toInt(2)
  }
  return getValue(vv, p + 1, comm)
}

fun main() {
  val v = System.`in`.bufferedReader().readLines()
  val ogr = getValue(v, 0, Commonality.MostCommon)
  val csr = getValue(v, 0, Commonality.LeastCommon)
  println(ogr * csr)
}
