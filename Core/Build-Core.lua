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
        "vendor/stb_image",
        "vendor/imgui"
    }
    
    defines
    {
        "PS_PLATFORM_WINDOWS",
        "PS_ENABLE_ASSERTS",
        "GLFW_INCLUDE_NONE"
    }
    
    links
    {
        "GLFW",
        "Glad",
        "GLM",
        "ImGui",
        "Image",
        "opengl32.lib"
    }
    
    targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir ("../Binaries-Intermediates/" .. OutputDir .. "/%{prj.name}")
    
    filter "system:windows"
        systemversion "latest"
        buildoptions { "/utf-8" }
        defines { }
    
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