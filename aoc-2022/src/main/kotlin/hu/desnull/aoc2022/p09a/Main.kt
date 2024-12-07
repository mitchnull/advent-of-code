package hu.desnull.aoc2022.p09a

import kotlin.math.abs
import kotlin.math.sign

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

data class Coords(val x: Int, val y: Int)

val Origin = Coords(0, 0)

val Moves = mapOf(
  'L' to Coords(-1, 0),
  'R' to Coords(+1, 0),
  'U' to Coords(0, +1),
  'D' to Coords(0, -1),
)

class State(
  var head: Coords = Origin,
  var tail: Coords = Origin,
  val visited: MutableSet<Coords> = mutableSetOf(Origin)
)

fun main() {
  val state = State()
  readLines().forEach { move(it, state) }
  println(state.visited.size)
}

fun move(spec: String, state: State) {
  val dir = spec[0]
  var count = spec.substring(2).toInt()
  while (count-- > 0) {
    move(Moves[dir] ?: Origin, state)
  }
}

fun move(move: Coords, state: State) {
  with(state) {
    head = add(head, move)
    tail = follow(head, tail)
    visited.add(tail)
  }
}

fun follow(head: Coords, tail: Coords): Coords {
  return when {
    d(head.x, tail.x) <= 1 && d(head.y, tail.y) <= 1 -> tail
    else -> Coords(tail.x + (head.x - tail.x).sign, tail.y + (head.y - tail.y).sign)
  }
}

fun add(a: Coords, b: Coords) = Coords(a.x + b.x, a.y + b.y)
fun d(a: Int, b: Int) = abs(a - b)
