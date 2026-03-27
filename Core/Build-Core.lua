project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"
    targetdir "Binaries/%{cfg.buildcfg}"
    staticruntime "off"
    
    files { "Source/**.h", "Source/**.cpp" }
    
    pchheader ( "pspch.h" )
    pchsource ( "Source/pspch.cpp" )
    
    includedirs
    {
        "Source",
        "vendor/GLFW/include",
        "vendor/Glad/include",
        "vendor/glm",
        "vendor/stb_image"
    }
    
    defines
    {
        "PS_ENABLE_ASSERTS",
        "GLFW_INCLUDE_NONE",
        "wxUSE_SOCKETS=0",
        "__WXMSW__",
        "_UNICODE", 
        "wxUSE_GUI=1",
        "wxUSE_IPV6=0",
        "WIN32_LEAN_AND_MEAN",
        "_WINSOCKAPI_"
    }
    
    filter "system:windows"
        links { "GLFW", "Glad", "GLM", "Image", "opengl32.lib" }
        defines { "PS_PLATFORM_WINDOWS" }
        systemversion "latest"
        buildoptions { "/utf-8" }
    
    filter "system:linux"
        buildoptions { "`pkg-config --cflags glfw3`" }
        linkoptions { "`pkg-config --libs glfw3`" }
        links { "GL", "dl" }
        defines { "PS_PLATFORM_LINUX" }
    
    filter {}
    
    targetdir ("%{wks.location}/Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/Binaries-Intermediates/" .. OutputDir .. "/%{prj.name}")
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"
    
    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"