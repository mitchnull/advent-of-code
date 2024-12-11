package hu.desnull.aoc2024.p11

import java.lang.Math.pow
import kotlin.math.log10

data class Block(val id: Int, val len: Int)

fun main() {
  val counts: MutableMap<Long, Long> = readln().split(" ").map { Pair(it.toLong(), 1L) }.toMap(HashMap())

  val after1 = solve(counts, 25)
  val after2 = solve(after1, 50)
  val res1 = after1.values.reduce { a, b -> a + b }
  val res2 = after2.values.reduce { a, b -> a + b }
  println("1: $res1")
  println("2: $res2")
}

fun solve(counts: Map<Long, Long>, rounds: Int): Map<Long, Long> {
  var curr = counts
  for (i in 1..rounds) {
    val next = HashMap<Long, Long>()
    curr.forEach { (n, c) ->
      if (n == 0L) {
        next.compute(1) { _, v -> (v ?: 0) + c }
      } else {
        val d = log10(n.toDouble()).toInt() + 1
        if (d % 2 == 0) {
          val m = pow(10.0, (d / 2.0)).toLong()
          next.compute(n / m) { _, v -> (v ?: 0) + c }
          next.compute(n % m) { _, v -> (v ?: 0) + c }
        } else {
          next.compute(n * 2024) { _, v -> (v ?: 0) + c }
        }
      }
    }
    curr = next
  }
  return curr
}
