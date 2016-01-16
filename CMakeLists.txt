cmake_minimum_required(VERSION 3.4)

project(LB/cloning
	VERSION 0.0.0
	LANGUAGES CXX
)
set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/${PROJECT_VERSION})

add_library(cloning INTERFACE)
#set_property(TARGET cloning PROPERTY CXX_STANDARD 17)
target_compile_options(cloning
	INTERFACE
		"-std=c++1z"
)
target_include_directories(cloning
	INTERFACE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
		$<INSTALL_INTERFACE:include>
)

install(
	TARGETS
		cloning
	EXPORT cloning
)
install(
	EXPORT cloning
	DESTINATION cmake/LB
	NAMESPACE LB::
)
install(
	FILES
		"cloning.hpp"
	DESTINATION include/${PROJECT_NAME}
)

enable_testing()

add_executable(test-basic
	"test/basic.cpp"
)
target_link_libraries(test-basic
	PUBLIC
		cloning
)
add_test(
	NAME basic
	COMMAND test-basic
)
