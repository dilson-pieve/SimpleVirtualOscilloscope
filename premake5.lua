newoption 
{
   trigger = "opengl43",
   description = "use OpenGL 4.3"
}

workspace "virtual-oscilloscope"
	configurations { "Debug", "Release" }
	platforms { "x64", "x86"}
	startproject "virtual-oscilloscope"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"	
	
	targetdir "bin/%{cfg.buildcfg}/"

project "raylib"
		filter "configurations:Debug OR Release"
			kind "StaticLib"
			
		filter "action:vs*"
			defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS"}
			characterset ("MBCS")
		
		filter "system:windows"
			defines{"_WIN32"}
			links {"winmm", "kernel32", "opengl32", "kernel32", "gdi32"}
			
		filter "system:linux"
			links {"pthread", "GL", "m", "dl", "rt", "X11"}
			
		filter{}
		
		defines{"PLATFORM_DESKTOP"}
		if (_OPTIONS["opengl43"]) then
			defines{"GRAPHICS_API_OPENGL_43"}
		else
			defines{"GRAPHICS_API_OPENGL_33"}
		end
	
		location "build"
		language "C"
		targetdir "bin/%{cfg.buildcfg}"
		
		includedirs { "raylib/src", "raylib/src/external/glfw/include"}
		vpaths 
		{
			["Header Files"] = { "raylib/src/**.h"},
			["Source Files/*"] = {"raylib/src/**.c"},
		}
		files {"raylib/src/*.h", "raylib/src/*.c"}
		
project "virtual-oscilloscope"
	kind "ConsoleApp"
	location "%{wks.name}"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	cppdialect "C++20"
	
	vpaths 
	{
		["Header Files"] = { "**.h", "**.hpp"},
		["Source Files"] = {"**.c", "**.cpp"},
	}
	files {"%{wks.name}/**.c", "%{wks.name}/**.cpp", "%{wks.name}/**.h", "%{wks.name}/**.hpp"}

	links {"raylib"}
	
	includedirs { "%{wks.name}", "%{wks.name}/Core", "raylib/src" }
	
	defines{"PLATFORM_DESKTOP"}
	if (_OPTIONS["opengl43"]) then
		defines{"GRAPHICS_API_OPENGL_43"}
	else
		defines{"GRAPHICS_API_OPENGL_33"}
	end
	
	filter "action:vs*"
		defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
		dependson {"raylib"}
		links {"raylib.lib"}
        characterset ("MBCS")
		
	filter "system:windows"
		defines{"_WIN32"}
		links {"winmm", "kernel32", "opengl32", "kernel32", "gdi32"}
		libdirs {"bin/%{cfg.buildcfg}"}
		
	filter "system:linux"
		links {"pthread", "GL", "m", "dl", "rt", "X11"}