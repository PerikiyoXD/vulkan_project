add_rules("mode.debug", "mode.release")

add_requires("vulkansdk", "glfw")

set_languages("cxx20")

-- Define a custom rule to compile GLSL shaders to SPIR-V
rule("compile_shaders")
    set_extensions(".vert", ".frag")

    on_build_file(function (target, sourcefile)
        print("Compiling shader for file: " .. sourcefile)
        local outputdir = path.join(target:targetdir(), "shaders")
        print("Creating output directory: " .. outputdir)
        os.mkdir(outputdir)
        local outputfile = path.join(outputdir, path.filename(sourcefile) .. ".spv")
        print("Output file: " .. outputfile)

        -- Execute the command and capture the result
        local compile_command = {"-V", sourcefile, "-o", outputfile}
        local ret = os.execv("glslangValidator", compile_command)
        if ret ~= 0 then
            os.raise("Failed to compile shader: " .. sourcefile .. " with error code: " .. ret)
        else
            print("Shader compiled successfully: " .. outputfile)
        end
    end)

    on_clean(function (target)
        local outputdir = path.join(target:targetdir(), "shaders")
        print("Cleaning directory: " .. outputdir)
        os.rmdir(outputdir)
    end)
rule_end()

-- Combine everything into a single target
target("vulkan_hello")
    set_kind("binary")
    add_files("src/**.cpp")
    add_files("shaders/spinning_circle.vert", "shaders/spinning_circle.frag")
    add_includedirs("include/Vulkan")
    add_packages("vulkansdk", "glfw")
    add_rules("compile_shaders")
