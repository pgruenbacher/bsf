# Source files and their filters
# include(CMakeSources.cmake)


file(GLOB SRCS
	ThirdParty/imgui/*.cpp
	ThirdParty/imgui-Guizmo/*.cpp
	Src/*.cpp
)


add_library(bsfImgui SHARED ${SRCS})


add_common_flags(bsfImgui)

target_include_directories(bsfImgui PUBLIC
	ThirdParty/imgui
	ThirdParty/imgui-Guizmo
)

target_include_directories(bsfImgui PUBLIC
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Source/Plugins>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Source/Foundation/bsfUtility>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Source/Foundation/bsfCore>
	$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/Source/Foundation/bsfEngine>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfUtility>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/Generated/bsfEngine>
	# $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfUtility>
	# $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfCore>
	# $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/bsfEngine>
)