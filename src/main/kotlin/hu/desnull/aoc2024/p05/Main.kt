package hu.desnull.aoc2024.p05

import java.util.Collections

fun check1(rules: Set<String>, list: List<String>): Boolean {
  return list.zipWithNext {
    a, b ->
    rules.contains("$b|$a")
  }.none { it }
}

fun order(rules: Set<String>, list: List<String>): List<String> {
  Collections.sort(list) {
    a, b ->
    if (rules.contains("$a|$b")) -1 else if (rules.contains("$b|$a")) 1 else 0
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
