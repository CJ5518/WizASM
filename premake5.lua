

local name = "WizASM"

workspace(name)
configurations({"debug","release"})

	project(name)
		kind("ConsoleApp")
		language("C++")
		targetdir("bin/%{cfg.buildcfg}")

		files({ "**.h", "**.cpp" })
		links {"SDL3"}


filter "configurations:Debug"
	defines { "DEBUG" }
	symbols "On"

filter "configurations:Release"
	defines { "NDEBUG" }
	optimize "On"
