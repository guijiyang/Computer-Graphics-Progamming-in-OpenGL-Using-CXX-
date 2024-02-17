add_packages("glfw", "opengl", "glew", "glm", "easyloggingpp")

-- add_defines("NOMINMAX")
-- macro that tells GLFW to expose the Win32 API symbols
-- add_defines("GLFW_EXPOSE_NATIVE_WIN32")

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
    add_links("Shlwapi")
    if not is_kind("shared") then
      if is_mode("debug") then
        add_ldflags("/OPT:NOREF") --[[ This flag disables elimination of unreferenced data. ]]
        add_ldflags("/OPT:NOICF") --[[ No Identical COMDAT folding. ]]
      else
        add_ldflags("/OPT:REF") --[[ Remove unreferenced functions and data. ]]
        add_ldflags("/OPT:ICF") --[[ Identical COMDAT folding. ]]
      end
    end
    -- on_load(function(target)
    --   if items[1] == "ch06_03" then
    --     target:add("packages", "assimp")
    --   end
    -- end)
    -- print(items[3])
    after_build(function(target)
      -- copy shader
      os.trycp(items[3], target:targetdir())
      -- copy config file
      os.trycp("$(projectdir)/src/opengltest.conf", target:targetdir())
      -- copy texture image
      -- if items[1] ~= "ch06_03" then
      --   os.trycp("$(projectdir)/texture", target:targetdir())
      -- end
      -- -- copy model
      -- if items[1] == "ch06_03" then
      --   os.trycp("$(projectdir)/model", target:targetdir())
      -- end
    end)
  end
  -- print(items[2])
end
