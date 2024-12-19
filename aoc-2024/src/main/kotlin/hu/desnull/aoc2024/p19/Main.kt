package hu.desnull.aoc2024.p19

fun countMatches(text: String, patterns: List<String>, cache: MutableMap<String, Long>): Long {
  if (text.isEmpty()) {
    return 1
  }
  val cached = cache[text]
  if (cached != null) {
    return cached
  }
  val res: Long = patterns.filter(text::startsWith).sumOf { countMatches(text.substring(it.length), patterns, cache) }
  cache[text] = res
  return res
}

fun main() {
  val line = readln()
  val patterns = line.split(", ").toList()
  readln()
  val targets = generateSequence { readlnOrNull() }.toList()

  val counts = targets.map { countMatches(it, patterns, HashMap()) }.toList()
  val res1 = counts.count { it > 0 }
  val res2 = counts.sum()
  println("1: $res1")
  println("2: $res2")
}
