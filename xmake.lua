set_xmakever("2.7.7")
set_policy("build.warning", true)
set_warnings("all","error")
set_exceptions("cxx")
set_languages("cxxlatest")

add_rules("mode.debug", "mode.release","mode.releasedbg")

add_requireconfs("*", {system=false})
add_requires("glfw", "opengl","glew","glm","easyloggingpp")
-- add_requires("easyloggingpp",{debug=true})

-- add_requires("opengl")
-- add_requires("")
includes("**/xmake.lua")

add_includedirs("src")
