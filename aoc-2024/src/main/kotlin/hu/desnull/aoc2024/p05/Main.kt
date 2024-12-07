package hu.desnull.aoc2024.p05

import java.util.Collections

fun check1(list: List<String>, rules: Set<String>): Boolean {
  for ((i, v) in list.withIndex()) {
    for (j in i + 1 until list.size) {
      if (rules.contains(list[j] + "|" + v)) {
        return false
      }
    }
  }
  return true
}

fun sort(list: List<String>, rules: Set<String>): List<String> {
  var needCheck = true
  while (needCheck) {
    needCheck = false
    for (i in list.indices) {
      for (j in i + 1 until list.size) {
        if (rules.contains(list[j] + "|" + list[i])) {
          Collections.swap(list, i, j)
          needCheck = true
        }
      }
    }
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
