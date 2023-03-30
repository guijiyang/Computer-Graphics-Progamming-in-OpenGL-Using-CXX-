add_packages("glfw", "opengl", "glew", "glm", "easyloggingpp")

add_defines("NOMINMAX")
add_cxxflags("cl::/wd4819")
local curdir = os.curdir()
local major = path.relative(curdir, "../")
set_group(major)

local targets = all_targets()
for _, items in ipairs(targets) do
  target(items[1])
  do
    set_kind("binary")
    add_files(items[2])
    add_deps("Utils")
    set_targetdir("$(projectdir)/build/" .. items[1])

    on_load(function(target)
      if items[1] == "ch06_03" then
        target:add("packages", "assimp")
      end
    end)
    -- print(items[3])
    after_build(function(target)
      -- copy shader
      os.trycp(items[3], target:targetdir())
      -- copy config file
      os.trycp("$(projectdir)/src/opengltest.conf", target:targetdir())
      -- copy texture image
      if items[1] ~= "ch06_03" then
        os.trycp("$(projectdir)/texture", target:targetdir())
      end
      -- copy model
      if items[1] == "ch06_03" then
        os.trycp("$(projectdir)/model", target:targetdir())
      end
    end)
  end
  -- print(items[2])
end
