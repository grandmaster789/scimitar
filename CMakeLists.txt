cmake_minimum_required (VERSION 3.16) # some of the dependencies have much more strict requirements

project ("scimitar" LANGUAGES CXX)

# all targets should use these settings
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/../lib")

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/../bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/../bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${PROJECT_BINARY_DIR}/../lib")
endforeach()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# mostly for debugging dependency issues when used within an IDE
message(STATUS "**** CMAKE_TOOLCHAIN_FILE *> ${CMAKE_TOOLCHAIN_FILE} <*")

# platform specific
if (WIN32)
	# compiler flags
	add_definitions(-DWIN32_LEAN_AND_MEAN)

	add_compile_options("/MP")          # build with multiple processes
	add_compile_options("/W4")          # high warning level warnings
	add_compile_options("/permissive-") # strict adherence to the c++ standard
	add_compile_options("/utf-8")       # set source and executable character sets to UTF-8
elseif(UNIX)
	# GCC 8 or higher is required
	add_definitions(-DUNIX)

	add_compile_options("-fvisibility=hidden")
endif()

# this needs to be active before adding the unittest subproject (if you want to have unit tests)
enable_testing()

add_subdirectory("app")        # executable application layer
add_subdirectory("scimitar")   # main library
add_subdirectory("unittest")   # unit tests (Google Test)