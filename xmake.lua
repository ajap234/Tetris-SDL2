add_requires("libsdl 2", "libsdl_image 2", "libsdl_ttf >= 2.0.18", "libsdl_mixer 2")
add_requires("libsdl 2", { configs = { shared = true } })
target("tetris")
	set_kind("binary")
	add_packages("libsdl", "libsdl_image", "libsdl_ttf", "libsdl_mixer")
	add_files("src/*.cpp", {includedirs = "inc", languages = "c++17"})