project "Fire"
	kind "SharedLib"
	---kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	pchheader "src/frpch.h"
	pchsource "src/frpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"%{IncludeDir.stb_image}/**.h",
		"%{IncludeDir.stb_image}/**.cpp"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml}"
	}
	--- "%{IncludeDir.ImGui}/backends",

	links
	{
		"glfw", "glad", "ImGui"
	}
---		"GL"
---		"GLAD"
---		"imgui"

	linkoptions {"-framework OpenGL","-framework Cocoa"}
---,"-framework IOKit","-framework CoreVideo","-v"}

	filter "system:windows"
		systemversion "latest"
		defines
		{
		}
		
	filter "system:macosx"
		systemversion "11"
		defines
		{
		}
		
	filter "configurations:Debug"
		defines "FR_DEBUG"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release"
		defines "FR_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "FR_DIST"
		runtime "Release"
		optimize "On"
