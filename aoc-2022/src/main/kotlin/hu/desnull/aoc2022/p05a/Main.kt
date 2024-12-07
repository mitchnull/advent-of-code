package hu.desnull.aoc2022.p05a

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

typealias Stack = ArrayList<Char>

fun readStacks(): List<Stack> {
  val stacks = ArrayList<Stack>()
  readLines().takeWhile {
    it.isNotEmpty()
  }.forEach {
    var i = 0
    var j = 1
    while (j < it.length) {
      val c = it[j]
      if (c >= 'A') {
        while (stacks.size <= i) {
          stacks.add(Stack())
        }
        stacks[i].add(0, c)
      }
      ++i
      j += 4
    }
  }
  return stacks
}

fun move(stacks: List<Stack>, count: Int, from: Int, to: Int) {
  for (c in 1..count) {
    stacks[to].add(stacks[from].removeLast())
  }
}

fun show(stacks: List<ArrayList<Char>>) {
  for (stack in stacks) {
    print(if (stack.isEmpty()) ' ' else stack.last())
  }
  println()
}

fun main() {
  val stacks = readStacks()
  readLines().forEach {
    val params = it.split(" ")
    move(stacks, params[1].toInt(), params[3].toInt() - 1, params[5].toInt() - 1)
  }
  show(stacks)
}
