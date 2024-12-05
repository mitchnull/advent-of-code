package hu.desnull.aoc2024.p05

import java.util.Collections

fun check1(list: List<String>, rules: Set<String>): Boolean =
  list.zipWithNext {
    a, b ->
    rules.contains("$b|$a")
  }.none { it }

fun sort(list: List<String>, rules: Set<String>): List<String> {
  Collections.sort(list) {
    a, b ->
    if (rules.contains("$a|$b")) -1 else if (rules.contains("$b|$a")) 1 else 0
  }
  return list
}

fun middle(list: List<String>): Int = list[list.size / 2].toInt()

fun main() {
  val rules = generateSequence { readlnOrNull() }.takeWhile { it.isNotBlank() }.toSet()
  val lists = generateSequence { readlnOrNull() }.map { it.split(",") }.toList()

  val res1 = lists.filter { check1(it, rules) }.sumOf { middle(it) }
  println("1: $res1")

  // ------------------------------------------------------------------------ //

  val res2 = lists.filter { !check1(it, rules) }.map { sort(it, rules) }.sumOf { middle(it) }
  println("2: $res2")
}
