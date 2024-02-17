set_xmakever("2.7.7")
set_policy("build.warning", true)
set_warnings("all", "error")
set_exceptions("cxx")
set_languages("cxxlatest")
-- set_languages("c11")

add_rules("mode.debug", "mode.release", "mode.releasedbg")

add_requireconfs("*", { system = false })
add_requires("glfw 3.3.8", "opengl", "glew 2.2.0", "glm 1.0.0", "easyloggingpp", "assimp 5.2.5", "yaml-cpp 0.7.0")
add_requires("lua 5.4.4")
-- set_policy("package.precompiled", false)
-- add_requires("easyloggingpp",{debug=true})

add_defines("STB_IMAGE_IMPLEMENTATION", "STB_IMAGE_WRITE_IMPLEMENTATION")
add_defines("GLM_ENABLE_EXPERIMENTAL")
-- add_requires("opengl")
-- includes( "xmake/Utils.lua")
add_includedirs("$(projectdir)/src")
includes("xmake/Utils.lua", "**/xmake.lua")

