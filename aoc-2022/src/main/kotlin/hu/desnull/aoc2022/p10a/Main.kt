package hu.desnull.aoc2022.p10a

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

const val N = 6
const val M = 40
const val R = 20

class State(
  val x: Int,
  val xx: Int,
  val cycleStart: Int,
  val cycleEnd: Int
) {

  fun signalStrength(): Int? {
    for (i in cycleStart..cycleEnd) {
      if (i % M == R) {
        return i * x
      }
    }
    return null
  }
}

fun main() {
  var state = State(1, 1, 0, 0)
  val res =
    readLines()
      .map {
        state = parse(it, state)
        state
      }
      .map {
        it.signalStrength()
      }
      .filterNotNull()
      .take(N).sum()
  println(res)
}

fun parse(line: String, state: State): State {
  return when {
    line.startsWith("addx ") -> State(state.xx, state.xx + line.substring("addx ".length).toInt(), state.cycleEnd + 1, state.cycleEnd + 2)
    else -> State(state.xx, state.xx, state.cycleEnd + 1, state.cycleEnd + 1)
  }
}
