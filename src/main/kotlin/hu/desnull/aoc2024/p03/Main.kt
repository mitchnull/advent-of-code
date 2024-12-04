package hu.desnull.aoc2024.p03

fun main() {
  val text = String(System.`in`.readAllBytes(), Charsets.UTF_8)
  val regex = Regex("mul[(]([0-9]{1,3}),([0-9]{1,3})[)]")

  val res1 = regex.findAll(text).map {
    it.groupValues[1].toInt() * it.groupValues[2].toInt()
  }.sum()
  println("1: $res1")

  // ------------------------------------------------------------------------ //

  val text2 = text.replace("don't[(][)].*?(do[(][)]|$)".toRegex(RegexOption.DOT_MATCHES_ALL), "")
  val res2 = regex.findAll(text2).map {
    it.groupValues[1].toInt() * it.groupValues[2].toInt()
  }.sum()
  println("2: $res2")
}
