# Source files and their filters
# include(CMakeSources.cmake)


file(GLOB SRCS
	ThirdParty/imgui/*.cpp
	ThirdParty/imgui-Guizmo/*.cpp
	Src/*.cpp
)

list(FILTER SRCS EXCLUDE REGEX ".*test.cpp")

add_library(bsfImgui SHARED ${SRCS})

add_common_flags(bsfImgui)

target_include_directories(bsfImgui PUBLIC
	ThirdParty/imgui
	ThirdParty/imgui-Guizmo
)

target_link_libraries(bsfImgui bsf)
