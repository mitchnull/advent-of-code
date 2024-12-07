package hu.desnull.aoc2022.p07b

fun isWhiteSpace(c: Char) = c in " \r\n\t"
fun readString() = generateSequence {
  val c = System.`in`.read()
  if (c != -1) c.toChar() else null
}.dropWhile { isWhiteSpace(it) }.takeWhile { !isWhiteSpace(it) }.joinToString("").ifEmpty { null }

fun readLines() = generateSequence {
  readlnOrNull()
}

const val TOTAL = 70000000
const val NEEDED = 30000000

enum class Cmd(val cmd: String, val len: Int = cmd.length + 1) {
  CD("$ cd"),
  LS("$ ls"),
}

enum class Type {
  File,
  Dir
}

class Node(
  val type: Type,
  val name: String,
  val parent: Node?,
  var size: Long,
  val children: MutableMap<String, Node>,
) {
  constructor(type: Type, name: String, parent: Node?, size: Long) : this(type, name, parent, size, HashMap<String, Node>()) {}
  companion object {
    fun dir(name: String, parent: Node?) = Node(Type.Dir, name, parent, -1)
    fun file(name: String, parent: Node?, size: Long) = Node(Type.File, name, parent, size)
  }
}

fun main() {
  val root = Node.dir("/", null)
  var cwd = root
  val lines = readLines()
  lines.forEach {
    cwd = parse(root, cwd, lines, it)
  }
  val du = dfs(root).last().size
  val df = TOTAL - du
  val minSize = NEEDED - df
  val res = dfs(root)
    .filter { it.type == Type.Dir }
    .filter { it.size >= minSize }
    .sortedBy(Node::size)
    .first()
    .size
  println(res)
}

fun dfs(node: Node): Sequence<Node> {
  return sequence {
    var size = 0L
    for (n in node.children.values) {
      if (n.type == Type.File) {
        size += n.size
        yield(n)
      } else {
        yieldAll(dfs(n))
        size += n.size
      }
    }
    node.size = size
    yield(node)
  }
}

fun parse(root: Node, cwd: Node, lines: Sequence<String>, line: String): Node {
  return when {
    line.startsWith(Cmd.CD.cmd) -> cd(root, cwd, line.substring(Cmd.CD.len))
    line.startsWith(Cmd.LS.cmd) -> cwd
    else -> addChild(cwd, line)
  }
}

fun addChild(cwd: Node, line: String): Node {
  val (size, name) = line.split(' ', limit = 2)
  if (size == "dir") {
    cwd.children.putIfAbsent(name, Node.dir(name, cwd))
  } else {
    cwd.children.putIfAbsent(name, Node.file(name, cwd, size.toLong()))
  }
  return cwd
}

fun cd(root: Node, cwd: Node, dir: String): Node {
  return when (dir) {
    "/" -> root
    ".." -> cwd.parent ?: root
    else -> {
      val cd = cwd.children.getOrPut(dir) {
        Node.dir(dir, cwd)
      }
      return if (cd.type == Type.Dir) cd else cwd
    }
  }
}
