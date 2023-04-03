
target("Utils") do
  set_kind("object")
  -- set_targetdir()
  add_packages("glm","glew","easyloggingpp")
  add_packages("assimp",{public=true})
  add_files("*.cpp")
end