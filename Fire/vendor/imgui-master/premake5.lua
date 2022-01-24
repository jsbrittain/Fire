project "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/obj/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"*.h",
		"*.cpp"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter {"system:windows","configurations:Release"}
		buildoptions "/MT"
