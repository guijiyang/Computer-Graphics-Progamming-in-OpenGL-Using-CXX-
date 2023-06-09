set_xmakever("2.7.7")
set_policy("build.warning", true)
set_warnings("all", "error")
set_exceptions("cxx")
set_languages("cxxlatest")
-- set_languages("c11")

add_rules("mode.debug", "mode.release", "mode.releasedbg")

add_requireconfs("*", { system = false })
add_requires("glfw", "opengl", "glew", "glm", "easyloggingpp", "assimp", "yaml-cpp")
-- add_requires("easyloggingpp",{debug=true})

add_defines("STB_IMAGE_IMPLEMENTATION", "STB_IMAGE_WRITE_IMPLEMENTATION")
-- add_requires("opengl")
-- includes( "xmake/Utils.lua")
includes("xmake/Utils.lua", "**/xmake.lua")

add_includedirs("src")
