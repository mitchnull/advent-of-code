package hu.desnull.aoc2024.p05

import java.util.Collections

fun check1(rules: Set<String>, list: List<String>): Boolean {
  for ((i, v) in list.withIndex()) {
    for (j in i + 1 until list.size) {
      if (rules.contains(list[j] + "|" + v)) {
        return false
      }
    }
  }
  return true
}

fun order(rules: Set<String>, list: List<String>): List<String> {
  for (i in list.indices) {
    for (j in i + 1 until list.size) {
      if (rules.contains(list[j] + "|" + list[i])) {
        Collections.swap(list, i, j)
      }
    }
  }
  return list
}

fun main() {
  val rules = generateSequence { readlnOrNull() }.takeWhile { it.isNotBlank() }.toSet()
  val lists = generateSequence { readlnOrNull() }.map { it.split(",") }.toList()

  val res1 = lists.filter { check1(rules, it) }.sumOf { it[it.size / 2].toInt() }

  println("1: $res1")

  // ------------------------------------------------------------------------ //

  val res2 = lists.filter { !check1(rules, it) }.map { order(rules, it) }.sumOf { it[it.size / 2].toInt() }
  println("2: $res2")
}
