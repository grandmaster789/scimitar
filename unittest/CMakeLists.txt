cmake_minimum_required (VERSION 3.16) # some of the dependencies have much more strict requirements

set(UNIT_TEST "unittest")

project(${UNIT_TEST} LANGUAGES CXX)

add_executable(${UNIT_TEST}
	"unittest.cpp"
)

find_package(GTest CONFIG REQUIRED)

include_directories(../scimitar)

target_link_libraries(${UNIT_TEST} PRIVATE
	GTest::gtest
	GTest::gtest_main
	GTest::gmock
	GTest::gmock_main
)

set_target_properties(${UNIT_TEST} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS OFF
)

add_test(NAME x COMMAND unittest)