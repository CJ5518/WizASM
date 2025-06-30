
local name = "WizASM"

workspace(name)

configurations({"debug","release"})

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"

project(name)
	kind("ConsoleApp")
	language("C++")
	targetdir("bin/%{cfg.buildcfg}")
	files({ "**.h", "**.cpp" })
	links {"SDL3", "libglew32", "OpenGL32"}
	includedirs {"include"}
