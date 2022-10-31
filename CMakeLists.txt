cmake_minimum_required(VERSION 3.21)

# ---- Project ----

project(
	ENBHelperSE
	VERSION 2.2.1
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)



set(Boost_USE_STATIC_LIBS ON)


# ---- Add source files ----

set(sources
	src/main.cpp
	src/functions.cpp
	src/PCH.h
	src/functions.h
)

source_group(
	TREE
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		${sources}
)

# ---- Dependencies ----

#find_package(spdlog REQUIRED CONFIG)
find_package(CommonLibSSE CONFIG REQUIRED)

# ---- Create DLL ----

add_commonlibsse_plugin(
	${PROJECT_NAME}
	"aers"
	SOURCES
	${sources}
)
add_library("${PROJECT_NAME}::${PROJECT_NAME}" ALIAS "${PROJECT_NAME}")

target_include_directories(${PROJECT_NAME}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/src>
        $<INSTALL_INTERFACE:src>
)


target_precompile_headers(
	${PROJECT_NAME}
	PRIVATE
		src/PCH.h
)
install(TARGETS ${PROJECT_NAME}
        DESTINATION "${CMAKE_INSTALL_LIBDIR}")
