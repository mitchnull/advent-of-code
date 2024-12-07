package hu.desnull.aoc2022.p04a

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

typealias Range = Pair<Int, Int>

fun containsOneWay(outer: Range, inner: Range) = outer.first <= inner.first && inner.second <= outer.second

fun contains(a: Range, b: Range) = containsOneWay(a, b) || containsOneWay(b, a)

fun <T> toPair(list: List<T>) = Pair(list[0], list[1])

fun parseRange(r: String): Range = toPair(r.split("-").map(String::toInt))

fun parse(line: String): Pair<Range, Range> = toPair(line.split(",").map(::parseRange))

fun main() {
  val res = readLines().map(::parse).count { contains(it.first, it.second) }
  println(res)
}
