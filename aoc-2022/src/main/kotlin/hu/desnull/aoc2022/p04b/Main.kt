package hu.desnull.aoc2022.p04b

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

typealias Range = Pair<Int, Int>

fun overlap(a: Range, b: Range) = !(a.second < b.first || b.second < a.first)

fun <T> toPair(list: List<T>) = Pair(list[0], list[1])

fun parseRange(r: String): Range = toPair(r.split("-").map(String::toInt))

fun parse(line: String): Pair<Range, Range> = toPair(line.split(",").map(::parseRange))

fun main() {
  val res = readLines().map(::parse).count { overlap(it.first, it.second) }
  println(res)
}
