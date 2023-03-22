set_xmakever("2.7.7")
set_policy("build.warning", true)
set_warnings("all","error")
set_exceptions("cxx")
set_languages("cxxlatest")

add_rules("mode.debug", "mode.release","mode.releasedbg")

add_requireconfs("*", {system=false})
add_requires("glfw", "opengl","glew")

-- add_requires("opengl")
-- add_requires("")
includes("**/xmake.lua")

-- if has_package("opengl") then
--   print("OpenGL founded")
-- end

