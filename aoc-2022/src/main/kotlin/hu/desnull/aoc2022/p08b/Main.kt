package hu.desnull.aoc2022.p08b

import java.lang.Integer.max

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

fun main() {
  val trees = readLines().map {
    it.chars().map { c -> c - '0'.code }.toArray()
  }.toList()
  val w = trees[0].size
  val h = trees.size
  val vis = Array(h) { IntArray(w) }
  var res = 0
  for (i in 0 until h) {
    for (j in 0 until w) {
      res = max(res, countScenery(trees, h, w, vis, i, j))
    }
  }
  println(res)
}

fun countScenery(trees: List<IntArray>, h: Int, w: Int, vis: Array<IntArray>, row: Int, col: Int): Int {
  var res = 1
  sequenceOf(-1, 1).forEach {
    res *= countScenery(trees, h, w, vis, row, col, 0, it)
    res *= countScenery(trees, h, w, vis, row, col, it, 0)
  }
  return res
}

fun countScenery(trees: List<IntArray>, h: Int, w: Int, vis: Array<IntArray>, row: Int, col: Int, di: Int, dj: Int): Int {
  val mh = trees[row][col]
  var res = 0
  var i = row + di
  var j = col + dj
  while (i in 0 until h && j in 0 until w) {
    ++res
    if (trees[i][j] >= mh) {
      break
    }
    i += di
    j += dj
  }
  return res
}
