package hu.desnull.aoc2024.p01

import kotlin.math.abs

fun main() {
  val left = ArrayList<Int>()
  val right = ArrayList<Int>()
  generateSequence { readlnOrNull() }.forEach {
    val (l, r) = it.split("\\s+".toRegex(), 2)
    left.add(l.toInt())
    right.add(r.toInt())
  }

  left.sort()
  right.sort()
  val res1 = left.zip(right).stream().map { abs(it.component1() - it.component2()) }.reduce(Integer::sum).orElse(0)
  println("1: $res1")

  // ------------------------------------------------------------------------ //

  val res2 = left.stream().map { it * right.count { r -> r == it } }.reduce(Integer::sum).orElse(0)
  println("2: $res2")
}
