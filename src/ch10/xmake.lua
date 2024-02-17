add_packages("glfw", "opengl", "glew", "glm", "easyloggingpp", "yaml-cpp", "lua")

-- add_defines("NOMINMAX")
-- macro that tells GLFW to expose the Win32 API symbols
-- add_defines("GLFW_EXPOSE_NATIVE_WIN32")

-- add_cxflags("cl::/wd4819 /wd4996")
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
    add_syslinks("Shlwapi")
    -- on_load(function(target)
    --   if items[1] == "ch06_03" then
    --     target:add("packages", "assimp")
    --   end
    -- end)
    -- print(items[3])
    after_build(function(target)
      -- copy shader
      os.trycp(items[3], target:targetdir())
      -- copy logger config file
      os.trycp("$(projectdir)/src/opengltest.conf", target:targetdir())
      -- -- copy texture image
      -- os.trycp("$(projectdir)/texture", target:targetdir())
      -- copy program config file
      os.trycp(items[4] .. "/config.yaml", target:targetdir())
    end)
  end
  -- print(items[2])
end
