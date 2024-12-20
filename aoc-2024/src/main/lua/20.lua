local function printf(s, ...)
 return io.write(s:format(...))
end

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

local function findPath(board, w, h, startPos)
  local path = {}
  local costs = {}
  local prevs = {}
  prevs[idx(startPos, w, h)] = pos(-1, -1)
  local queue = {}
  table.insert(queue, {pos = startPos, cost = 0})
  costs[idx(startPos, w, h)] = 0
  while #queue > 0 do
    local n = table.remove(queue, 1)
    if board[n.pos.y][n.pos.x] == 'E' then
      local p = n.pos
      while p.x ~= -1 do
        table.insert(path, p)
        p = prevs[idx(p, w, h)]
      end
      return path
    end
    if n.cost == costs[idx(n.pos, w, h)] then
      for _, d in ipairs(DIRS) do
        local nn = {pos = pos(n.pos.x + d.x, n.pos.y + d.y), cost = n.cost + 1}
        local cc = costs[idx(nn.pos, w, h)] or math.huge
        if board[nn.pos.y][nn.pos.x] ~= '#' and nn.cost < cc then
          costs[idx(nn.pos, w, h)] = nn.cost
          prevs[idx(nn.pos, w, h)] = n.pos
          table.insert(queue, nn)
        end
      end
    end
  end
  return path
end

local function solve(path, maxCheat1, maxCheat2, minGain)
  local res1, res2 = 0, 0
  for i = 1, #path do
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

print(math.huge)

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
printf("1: %d\n", res1)
printf("2: %d\n", res2)

