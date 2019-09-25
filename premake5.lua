workspace "TP"
    configurations { "Debug", "Release" }

    filter { "configurations:Debug" }
        symbols "On"
    filter {}

    filter { "configurations:Release" }
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
            "D:/msys64/mingw64/include"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "freeimage"
        }
        buildoptions { "-fopenmp" }
        linkoptions { "-fopenmp" }

    -- RTTest, unit testing of RT
    project "RTTest"
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
            "RT/src"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "RT"
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
            "RT/src"
        }
        libdirs {
            "D:/msys64/mingw64/lib"
        }
        links {
            "RT",
            "freeimage"
        }
        linkoptions { "-fopenmp" }
