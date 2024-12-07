package hu.desnull.aoc2022.p09b

import kotlin.math.abs
import kotlin.math.max
import kotlin.math.min
import kotlin.math.sign

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

const val N = 10

data class Coords(val x: Int, val y: Int)

val Origin = Coords(0, 0)

val Moves = mapOf(
  'L' to Coords(-1, 0),
  'R' to Coords(+1, 0),
  'U' to Coords(0, +1),
  'D' to Coords(0, -1),
)

class State(
  val rope: Array<Coords> = Array(N) { Origin },
  val visited: MutableSet<Coords> = mutableSetOf(Origin),
  var min: Coords = Origin,
  var max: Coords = Origin,
)

fun main() {
  val state = State()
  readLines().forEach { move(it, state) }
//  printVisited(state)
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
    rope[0] = add(rope[0], move)
    min = Coords(min(rope[0].x, min.x), min(rope[0].y, min.y))
    max = Coords(max(rope[0].x, max.x), max(rope[0].y, max.y))
    for (i in 1 until N) {
      rope[i] = follow(rope[i - 1], rope[i])
    }
    visited.add(rope.last())
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

fun printVisited(state: State) {
  for (y in state.max.y downTo state.min.y) {
    for (x in state.min.x..state.max.x) {
      if (x == 0 && y == 0) {
        print('s')
      } else if (state.visited.contains(Coords(x, y))) {
        print('#')
      } else {
        print('.')
      }
    }
    println()
  }
}
