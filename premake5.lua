workspace "Fire"
	architecture "x86_64"
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
IncludeDir["entt"] = "%{wks.location}/Fire/vendor/entt/include"
IncludeDir["yaml"] = "%{wks.location}/Fire/vendor/yaml-cpp/include"

flags
{
	"MultiProcessorCompile"
}

group "Dependencies"
---	include "Fire/vendor/GLFW"
	include "Fire/vendor/glad"
	include "Fire/vendor/imgui-docking"
	include "Fire/vendor/yaml-cpp"
group ""

include "Fire"
include "Sandbox"
include "FireEditor"
--include "Games/Flappy"