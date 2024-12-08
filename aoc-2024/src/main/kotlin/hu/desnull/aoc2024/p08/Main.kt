package hu.desnull.aoc2024.p08

data class Pos(val x: Int, val y: Int)

fun main() {
  val map = HashMap<Char, ArrayList<Pos>>()
  var y = 0
  var w = 0
  generateSequence { readlnOrNull() }.forEach {
    w = it.length
    for ((x, c) in it.withIndex()) {
      if (c != '.') {
        map.computeIfAbsent(c) { ArrayList() }.add(Pos(x, y))
      }
    }
    ++y
  }

  val res1 = solve1(map, w, y)
  println("1: $res1")

  val res2 = solve2(map, w, y)
  println("1: $res2")
}

fun solve1(map: Map<Char, List<Pos>>, w: Int, h: Int): Int {
  val antiNodes = HashSet<Pos>()
  map.forEach {
    for ((i, a) in it.value.withIndex()) {
      for (j in i + 1 until it.value.size) {
        val b = it.value[j]
        addNode(Pos(a.x - (b.x - a.x), a.y - (b.y - a.y)), antiNodes, w, h)
        addNode(Pos(b.x + (b.x - a.x), b.y + (b.y - a.y)), antiNodes, w, h)
      }
    }
  }
  return antiNodes.size
}

fun solve2(map: Map<Char, List<Pos>>, w: Int, h: Int): Int {
  val antiNodes = HashSet<Pos>()
  map.forEach {
    for ((i, a) in it.value.withIndex()) {
      for (j in i + 1 until it.value.size) {
        val b = it.value[j]
        addNodes(a, b.x - a.x, b.y - a.y, antiNodes, w, h)
        addNodes(a, a.x - b.x, a.y - b.y, antiNodes, w, h)
      }
    }
  }
  return antiNodes.size
}

fun addNodes(a: Pos, dx: Int, dy: Int, antiNodes: HashSet<Pos>, w: Int, h: Int) {
  var p = a
  while (addNode(p, antiNodes, w, h)) {
    p = Pos(p.x + dx, p.y + dy)
  }
}

fun addNode(pos: Pos, antiNodes: HashSet<Pos>, w: Int, h: Int): Boolean {
  if (pos.x in 0 until w && pos.y in 0 until h) {
    antiNodes.add(pos)
    return true
  }
  return false
}
