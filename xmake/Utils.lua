function all_targets()
  local targets = {}
  local curdir = os.curdir()
  local major = path.relative(curdir, "../")
  for _, dir in ipairs(os.dirs("./*")) do
    -- print(os.scriptdir().."/"..dir)
    local items = {}
    table.insert(items, major .. "_" .. dir)
    table.insert(items, dir .. "/*.cpp")
    table.insert(items, path.join(curdir, dir, "shader/*"))
    -- print(path.translate(curdir, dir, "shader/*"))
    table.insert(targets, items)
  end
  return targets
end
