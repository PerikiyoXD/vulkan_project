add_rules("mode.debug", "mode.release")

add_requires("vulkansdk", "glfw")

set_languages("cxx20")

target("vulkan_hello")
    set_kind("binary")
    add_files("src/*.cpp")
    add_includedirs("include")
    add_packages("vulkansdk", "glfw")
