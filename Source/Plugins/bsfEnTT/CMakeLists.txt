
file(GLOB_RECURSE SRCS
	Components/*.cpp
	RenderComponents/*.cpp
	Scene/*.cpp
)
list(FILTER SRCS EXCLUDE REGEX ".*test.cpp")


add_library(bsfEnTT SHARED ${SRCS})

# target_compile_features(bsfEnTT
#     PRIVATE
#     cxx_std_17
# )
# prevents the "undefined reference to typeinfo"
# target_compile_options(bsfEnTT PUBLIC -fno-rtti -fPIC)

add_common_flags(bsfEnTT)

target_include_directories(bsfEnTT PUBLIC "ThirdParty/EnTT/src")
target_include_directories(bsfEnTT PUBLIC ".")

target_link_libraries(bsfEnTT bsf)