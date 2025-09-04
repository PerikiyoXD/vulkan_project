add_rules("mode.debug", "mode.release")

add_requires("vulkansdk")
add_requires("glfw")

set_languages("cxx23")

target("vulkan_hello")
    set_kind("binary")
    add_files("src/*.cpp")
    add_files("src/Vulkan/*.cpp")
    add_includedirs("include", {public = true})
    add_packages("vulkansdk")
    add_packages("glfw")