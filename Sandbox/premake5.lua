project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")

	linkoptions {"-framework OpenGL","-framework Cocoa","-framework IOKit","-framework CoreVideo"}


	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Fire/vendor/spdlog/include",
		"%{wks.location}/Fire/vendor/glad",
		"%{wks.location}/Fire/src",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Fire", "glfw", "glad"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:macosx"
		systemversion "11"

	filter "configurations:Debug"
		defines "FR_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "FR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "FR_DIST"
		optimize "On"
