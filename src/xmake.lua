
target("Utils") do
  set_kind("object")
  -- set_targetdir()
  add_packages("glew","easyloggingpp")
  add_packages("assimp",{public=true})
  add_files("Utils.cpp")
end