package hu.desnull.aoc2022.p10b

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

const val M = 40

class State(
  val x: Int,
  val xx: Int,
  val cycleStart: Int,
  val cycleEnd: Int
)

fun main() {
  var state: State? = State(1, 1, 0, 0)
  var c = 1
  var p = 0
  while (state != null) {
    state = getState(c, state)
    if (state == null) {
      return
    }
    if (p in state.x - 1..state.x + 1) {
      print("#")
    } else {
      print(".")
    }
    ++c
    p = (p + 1) % M
    if (p == 0) {
      println()
    }
  }
}

fun getState(c: Int, state: State): State? {
  var res = state
  while (res.cycleEnd < c) {
    val line = readlnOrNull() ?: return null
    res = parse(line, state)
  }
  return res
}

fun parse(line: String, state: State): State {
  return when {
    line.startsWith("addx ") -> State(state.xx, state.xx + line.substring("addx ".length).toInt(), state.cycleEnd + 1, state.cycleEnd + 2)
    else -> State(state.xx, state.xx, state.cycleEnd + 1, state.cycleEnd + 1)
  }
}
