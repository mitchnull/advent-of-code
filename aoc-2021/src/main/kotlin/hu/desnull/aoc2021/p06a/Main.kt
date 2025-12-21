package hu.desnull.aoc2021.p06a

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

val N = 7
val DAYS = 80
typealias Ages = IntArray

val ages = Ages(N + 2) { 0 }

fun idx(i: Int, offset: Int): Int {
  return (offset + i) % N
}

fun tick(ages: Ages, tick: Int) {
  val zi = idx(0, tick)
  val hatching = ages[zi]
  ages[zi] += ages[N]
  ages[N] = ages[N + 1]
  ages[N + 1] = hatching
}

fun main() {
  readln().split(",").stream().map(String::toInt).forEach {
    ++ages[it]
  }
  for (i in 0 until DAYS) {
    tick(ages, i)
  }
  println(ages.sum())
}
