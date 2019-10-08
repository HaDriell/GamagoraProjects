workspace "TP"
    configurations { "debug", "release" }

    warnings "Default"

    filter { "configurations:debug" }
        symbols "On"
    filter {}

    filter { "configurations:release" }
        optimize "On"
    filter {}

    -- Glad is an OpenGL Loader. Currently loading OpenGL 4.6 without any manufacturer specific extension
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

    -- Utility project that replaces GLM 
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
    -- Utility project for any useful classes
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

    -- ChuteLibre is the first TP of this year. It's just a small simulation about falling objects, gravity and some basic physics
    project "ChuteLibre"
        location "%{prj.name}"
        language "C++"
        architecture "x64"
        kind "ConsoleApp"
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

    -- RT, a little raytracing lib
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

    --Global unit testing project for all the workspace
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
            "Math/src"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "AI",
            "Math"
        }
    
    -- RTSandbox, well... a sandbox
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

    -- AI is containing mostly all the tools I had to develop during my AI courses
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

    -- AI Showoff
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