package hu.desnull.aoc2022.p03b

import java.util.stream.Collectors

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

fun charSet(str: String): Set<Char> {
  return str.chars().mapToObj { Char(it) }.collect(Collectors.toSet())
}

fun prio(c: Char): Int {
  return if (c > 'Z') c - 'a' + 1 else c - 'A' + 27
}

fun check(group: List<String>): Int {
  return group.map(::charSet).reduce { a, b -> a.intersect(b) }.sumOf { prio(it) }
}

fun readGroups() = generateSequence { readLines().take(3).toList().ifEmpty { null } }

fun main() {
  val res = readGroups().map(::check).sum()
  println(res)
}
