workspace "TP"
    configurations { "debug", "release" }

    warnings "Default"

    filter { "configurations:debug" }
        symbols "On"
    filter {}

    filter { "configurations:release" }
        optimize "On"
    filter {}

-------------------------------------------------------------------------------
-- Tools & Utility
-------------------------------------------------------------------------------

    -- Math library that aims to be directly compatible with OpenGL and provide 
    -- basic vector math capabilities.
    project "Math"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "StaticLib"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
        }
        
    -- Utility project. Aims to store most of the boiler plate code that is likely
    -- to be used in any project. 
    project "Util"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "StaticLib"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
        }

-------------------------------------------------------------------------------
-- Real Time Rendering related projects
-------------------------------------------------------------------------------

    -- Glad is an OpenGL loader. The sources are generated from a website.
    -- OpenGL version loaded   : OpenGL 4.6
    -- Manufacturer extensions : None
    project "Glad"
        location "%{prj.name}"
        language "C"
        architecture "x64"
        kind "StaticLib"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "%{prj.name}/include"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
        }

    -- GL aims to be a small real time rendering library. 
    -- It's inspired by Hazel 
    project "GL"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "StaticLib"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "Math/src",
            "Util/src",
            "Glad/include"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "glfw3",
            "freeimage",
            "Math",
            "Util",
            "Glad"
        }
    
        project "GLSandbox"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "ConsoleApp"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "Math/src",
            "Util/src",
            "Glad/include",
            "GL/src"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "GL",
            "glfw3",
            "freeimage",
            "Math",
            "Util",
            "Glad"
        }

    project "Guibou"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "ConsoleApp"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "Math/src",
            "Util/src",
            "Glad/include",
            "GL/src"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "GL",
            "glfw3",
            "freeimage",
            "Math",
            "Util",
            "Glad"
        }

-------------------------------------------------------------------------------
-- Ray Tracing
-------------------------------------------------------------------------------
    -- Static library implementing some basic ray tracing capabilities
    project "RT"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "StaticLib"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "Math/src",
            "Util/src"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "freeimage",
            "Math",
            "Util"
        }
        buildoptions { "-fopenmp" }
        linkoptions { "-fopenmp" }

    -- Example application using the RT static library to render something
    project "RTSandbox"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "ConsoleApp"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "RT/src",
            "Math/src",
            "Util/src"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "RT",
            "freeimage",
            "Math",
            "Util"
        }
        linkoptions { "-fopenmp" }

-------------------------------------------------------------------------------
-- Artificial Intelligence
-------------------------------------------------------------------------------

    -- This projects aims to be a useful library gathering most AI algorithms
    -- seen during class & courses.
    project "AI"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "StaticLib"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "Math/src"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "Math"
        }

    project "FANN"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "ConsoleApp"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "floatfann"
        }


    -- Project showing off some Pathfinding and Finite state machines
    project "Gamagochi"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "ConsoleApp"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "AI/src",
            "Math/src",
            "Util/src",
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "AI",
            "Math",
            "Util",
            "sfml-system",
            "sfml-window",
            "sfml-graphics",
            "sfml-audio",
        }
        defines {
            "_WIN32"
        }
-------------------------------------------------------------------------------
-- Unit Tests
-------------------------------------------------------------------------------
    project "UnitTest"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "ConsoleApp"
        cppdialect "gnu++17"
        files {
            "%{prj.name}/src/**"
        }
        includedirs {
            "D:/msys64/mingw64/include",
            "AI/src",
            "Math/src",
            "GL/src",
            "Util/src",
            "Glad/include"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "AI",
            "Math",
            "GL",
            "Util",
            "Glad",
            "glfw3",
            "freeimage"
        }