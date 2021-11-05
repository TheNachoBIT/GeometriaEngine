workspace "Geometria"
	configurations { "Release" }
	platforms { "x86_64", "x86" }
	startproject "Geometria"

IncludeDirectories = {}

include "Libraries/GLFW"
IncludeDirectories["GLFW"] = "Libraries/GLFW/GLFWCore/include/GLFW"

include "Libraries/GLEW"
IncludeDirectories["GLEW"] = "Libraries/GLEW/GLEWCore/include"

include "Libraries/YAMLCPP"
IncludeDirectories["YAMLCPP"] = "Libraries/YAMLCPP/YAMLCPPCore/include"

project "Geometria"
	kind "ConsoleApp"
	language "C++"
	location "%{prj.name}"
	targetdir "%{prj.name}"

	files 
	{ 
		"%{prj.name}/**.h", 
		"%{prj.name}/**.hpp", 
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"Libraries/include",
		"%{IncludeDirectories.GLFW}",
		"%{IncludeDirectories.GLEW}",
		"%{IncludeDirectories.YAMLCPP}"
	}
	
	filter "system:windows"
	   links
	   {
	    	"GLFW",
			"GLEW",
			"yaml-cpp",
			"opengl32.lib"
	   }

	filter "system:linux"
	   links
	   {
	   		"GLFW",
			"GLEW",
			"yaml-cpp",
			"GL",
			"pthread",
			"m",
			"dl",
			"stdc++fs"
	   }

	filter "configurations:Release"
		defines 
		{ 
			"_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING",
			"GLEW_STATIC",
			"NDEBUG",
			"_CONSOLE" 
		}
		optimize "On"

    filter "platforms:x86"
	if(system == "windows") then
		defines
		{
			"WIN32"
		}
	end
        architecture "x86"

    filter "platforms:x86_64"
        architecture "x86_64"

    filter "system:windows"
    	systemversion "latest"
