add_packages("glfw", "opengl", "glew","glm", "easyloggingpp")
add_cxxflags("/wd4819")
function all_targets()
  local targets = {}
  local curdir = os.curdir()
  local major = path.relative(curdir, "../")
  for _, dir in ipairs(os.dirs("./*")) do
    -- print(os.scriptdir().."/"..dir)
    local items = {}
    table.insert(items, major .. "_" .. dir)
    table.insert(items, dir .. "/*.cpp")
    table.insert(items, path.join(curdir,dir, "shader/*"))
    -- print(path.translate(curdir, dir, "shader/*"))
    table.insert(targets, items)
  end
  return targets
end

local targets = all_targets()
for _, items in ipairs(targets) do
  target(items[1])
  do
    set_kind("binary")
    add_files(items[2])
    add_files("../*.cpp")
    set_targetdir("$(projectdir)/build/" .. items[1])
    -- print(items[3])
    after_build(function(target)
      os.trycp(items[3], target:targetdir())
      -- print("$(projectdir)/src/opengltest.conf")
      os.trycp("$(projectdir)/src/opengltest.conf", target:targetdir())
      os.trycp("$(projectdir)/texture/*", target:targetdir().."/texture/")
    end)
  end
  -- print(items[2])
end
