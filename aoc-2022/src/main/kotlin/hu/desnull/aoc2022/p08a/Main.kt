package hu.desnull.aoc2022.p08a

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
    res += countVis(trees, h, w, vis, i, 0, 0, +1)
    res += countVis(trees, h, w, vis, i, w - 1, 0, -1)
  }
  for (j in 0 until w) {
    res += countVis(trees, h, w, vis, 0, j, +1, 0)
    res += countVis(trees, h, w, vis, h - 1, j, -1, 0)
  }
  println(res)
}

fun countVis(trees: List<IntArray>, h: Int, w: Int, vis: Array<IntArray>, row: Int, col: Int, di: Int, dj: Int): Int {
  var res = 0
  var mh = -1
  var i = row
  var j = col
  while (i in 0 until h && j in 0 until w) {
    if (trees[i][j] > mh) {
      mh = trees[i][j]
      if (vis[i][j]++ == 0) {
        ++res
      }
    }
    i += di
    j += dj
  }
  return res
}
