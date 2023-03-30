add_packages("glfw", "opengl", "glew")
local curdir = os.curdir()
local major = path.relative(curdir, "../")
set_group(major)

local targets = all_targets()
for _, items in ipairs(targets) do
  target(items[1])
  do
    set_kind("binary")
    add_files(items[2])
    set_targetdir("$(projectdir)/build/" .. items[1])
    -- print(items[3])
    after_build(function(target)
      os.trycp(items[3], target:targetdir())
    end)
  end
  -- print(items[2])
end
