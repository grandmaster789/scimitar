cmake_minimum_required (VERSION 3.16) # some of the dependencies have much more strict requirements

set(UNIT_TEST "unittest")

project(${UNIT_TEST} LANGUAGES CXX)

add_executable(${UNIT_TEST}
	"unittest.cpp"
	"test_shared.cpp"
	"test_e57.cpp"
	"test_normals.cpp"
	"test_segmentation.cpp"
)

find_package(GTest CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyply CONFIG REQUIRED)
find_package(PCL REQUIRED COMPONENTS common io kdtree features)
find_package(libLAS CONFIG REQUIRED)
find_package(GeoTIFF CONFIG REQUIRED)

include_directories(../e57-lib)
include_directories(../shared-lib)
include_directories(${PCL_INCLUDE_DIRS})

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

target_link_libraries(${UNIT_TEST} PRIVATE
	e57-lib
	aero-shared-lib
	glm
	tinyply
	${PCL_LIBRARIES}
	liblas
	liblas_c
	geotiff_library
	GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
)

# find_package(Eigen3 CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Eigen3::Eigen)

# 

set_target_properties(${UNIT_TEST} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS OFF
)

add_test(NAME x COMMAND unittest)