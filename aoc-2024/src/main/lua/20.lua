local function pos(x, y)
  return {x = x, y = y}
end

local DIRS = {
  pos(0, -1),
  pos(1, 0),
  pos(0, 1),
  pos(-1, 0),
}

local function findStart(lines, w, h)
  for y = 1, h do
    for x = 1, w do
      if lines[y][x] == 'S' then
        return pos(x, y)
      end
    end
  end
end

local function idx(pos, w, h)
  return pos.y * w + pos.x
end

local function findPath(board, w, h, p)
  local path = {}
  local prev = p
  while true do
    table.insert(path, p)
    if board[p.y][p.x] == 'E' then
      return path
    end
    for _, d in ipairs(DIRS) do
      local pp = pos(p.x + d.x, p.y + d.y)
      if board[pp.y][pp.x] ~= '#' and not (pp.x == prev.x and pp.y == prev.y) then
        prev, p = p, pp
        break
      end
    end
  end
end

local function solve(path, maxCheat1, maxCheat2, minGain)
  local res1, res2 = 0, 0
  for i = 1, #path - minGain do
    for j = i + minGain, #path do
      local d = math.abs(path[i].x - path[j].x) + math.abs(path[i].y - path[j].y)
      if (j - i) - d >= minGain then
        if d <= maxCheat1 then
          res1 = res1 + 1
        end
        if d <= maxCheat2 then
          res2 = res2 + 1
        end
      end
    end
  end
  return res1, res2
end

-- ------------------------------------------------------------------------ --

local board = {}
for line in io.lines() do
  row = {}
  line:gsub(".",function(c) table.insert(row, c) end)
  table.insert(board, row)
end

local w = #board[1]
local h = #board

local startPos = findStart(board, w, h)

local path = findPath(board, w, h, startPos)

local res1, res2 = solve(path, 2, 20, 100)
print("!:", res1)
print("2:", res2)

