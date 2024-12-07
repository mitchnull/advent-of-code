package hu.desnull.aoc2024.p02

fun check(list: List<Int>): Boolean {
  val diffs = list.zipWithNext() {
    a, b ->
    a - b
  }
  return diffs.all { it in 1..3 } || diffs.all { it in -3..-1 }
}

fun main() {
  val lines = generateSequence { readlnOrNull() }.toList()

  val res1 = lines
    .map { it.split("\\s+".toRegex()).map(String::toInt) }
    .count { check(it) }
  println("1: $res1")

  // ------------------------------------------------------------------------ //

  val res2 = lines
    .map { it.split("\\s+".toRegex()).map(String::toInt) }
    .map {
      (0..it.size).map { d ->
        check(it.filterIndexed { i, _ -> i != d })
      }.any { it }
    }.count { it }
  println("2: $res2")
}
