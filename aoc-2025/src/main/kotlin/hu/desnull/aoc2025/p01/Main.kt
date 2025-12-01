package hu.desnull.aoc2025.p01

const val N = 100

fun main() {
  var res1 = 0
  var res2 = 0
  var curr = 50
  generateSequence { readlnOrNull() }.forEach {
    val n = it.substring(1).toInt()
    res2 += n / N
    val r = if (it[0] == 'R') n % N else - (n % N)
    val next = (curr + r + N) % N
    if (next == 0) {
      ++res1
    }
    if ((curr != 0) && (next == 0 || (r > 0 && next < curr) || (r < 0 && next > curr))) {
      ++res2
    }
    curr = next
  }
  println("1: $res1")
  println("2: $res2")
}
