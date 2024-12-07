package hu.desnull.aoc2022.p03a

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

fun check(line: String): Int {
  val mid = line.length / 2
  val a = charSet(line.substring(0, mid))
  val b = charSet(line.substring(mid))
  val i = a.intersect(b)
  return i.sumOf { prio(it) }
}

fun main() {
  val res = readLines().map(::check).sum()
  println(res)
}
