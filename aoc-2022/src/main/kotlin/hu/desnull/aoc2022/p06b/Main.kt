package hu.desnull.aoc2022.p06b

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

const val N = 14
typealias Window = IntArray

fun isUniq(w: Window): Boolean {
  val set = hashSetOf(0)
  return w.all { set.add(it) }
}

fun main() {
  val w = Window(N)
  var i = 0
  while (true) {
    val c = System.`in`.read()
    if (c == -1) {
      return
    }
    w[++i % N] = c
    if (isUniq(w)) {
      return println(i)
    }
  }
}
