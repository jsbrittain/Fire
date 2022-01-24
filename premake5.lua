workspace "Fire"
	architecture "x64"
	startproject "Sandbox"

	--require "vscode"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Fire/vendor/GLFW/include"
IncludeDir["GLAD"] = "%{wks.location}/Fire/vendor/glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Fire/vendor/imgui-docking"
IncludeDir["glm"] = "%{wks.location}/Fire/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Fire/vendor/stb_image"

flags
{
	"MultiProcessorCompile"
}

group "Dependencies"
---	include "Fire/vendor/GLFW"
	include "Fire/vendor/glad"
	include "Fire/vendor/imgui-docking"
group ""

include "Fire"
include "Sandbox"
