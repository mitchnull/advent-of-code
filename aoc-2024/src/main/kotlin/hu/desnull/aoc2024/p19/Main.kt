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
  val pattern = ("^(" + line.replace(", ", "|") + ")+$").toRegex()
  val patterns = line.split(", ").toList()
  readln()
  val targets = generateSequence { readlnOrNull() }.toList()

  val res1 = targets.count(pattern::matches)
  val res2 = targets.sumOf { countMatches(it, patterns, HashMap()) }
  println("1: $res1")
  println("2: $res2")
}
