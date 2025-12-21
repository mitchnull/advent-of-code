package hu.desnull.aoc2021.p08b

private fun isWhiteSpace(c: Char) = c in " \r\n\t"
private fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

private fun readLines() = generateSequence {
  readlnOrNull()
}

fun segments(str: String): Set<Char> {
  return str.toCharArray().toSet()
}

fun decode(line: String): Int {
  val (testsStr, digitsStr) = line.split("|")
  val tests = testsStr.trim().split(" ")
  val guessed = guess(tests)
  val digits = digitsStr.trim().split(" ")
  var res = 0
  for (d in digits) {
    res = res * 10 + guessed[segments(d)]!!
  }
  return res
}

fun guess(tests: List<String>): Map<Set<Char>, Int> {
  val res = HashMap<Set<Char>, Int>()
  val bySize = tests.map(::segments).groupBy { it.size }
  val cfSegments = if (bySize[2]?.size == 1) bySize[2]?.first() else null
  val acfSegments = if (bySize[3]?.size == 1) bySize[3]?.first() else null
  val bcdfSegments = if (bySize[4]?.size == 1) bySize[4]?.first() else null
  val allSegments = if (bySize[7]?.size == 1) bySize[7]?.first() else null
  val bdSegments = if (cfSegments != null) bcdfSegments?.minus(cfSegments) else null
  if (cfSegments != null) {
    res[cfSegments] = 1
  }
  if (bcdfSegments != null) {
    res[bcdfSegments] = 4
  }
  if (acfSegments != null) {
    res[acfSegments] = 7
  }
  if (allSegments != null) {
    res[allSegments] = 8
  }
  if (bySize[5] != null) {
    for (fs in bySize[5]!!) {
      if (cfSegments != null && fs.containsAll(cfSegments)) {
        res[fs] = 3
      } else if (bdSegments != null) {
        if (fs.containsAll(bdSegments)) {
          res[fs] = 5
        } else {
          res[fs] = 2
        }
      }
    }
  }
  if (bySize[6] != null) {
    for (fs in bySize[6]!!) {
      if (cfSegments != null) {
        if (fs.containsAll(cfSegments)) {
          if (bcdfSegments != null) {
            if (fs.containsAll(bcdfSegments)) {
              res[fs] = 9
            } else {
              res[fs] = 0
            }
          }
        } else {
          res[fs] = 6
        }
      } else if (bcdfSegments != null && fs.containsAll(bcdfSegments)) {
        res[fs] = 9
      }
    }
  }
  return res
}

fun main() {
  println(readLines().map(::decode).sum())
}
