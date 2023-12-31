project(FileSizeExplorer_Test)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(${PROJECT_NAME} "FileIteratorTest.cpp" "HeapTest.cpp")

target_include_directories(${PROJECT_NAME} 
	PUBLIC "../src"
)

target_link_libraries(${PROJECT_NAME} 
	GTest::gtest_main
	FileSizeExplorer_Lib
)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/test/data.txt
                ${CMAKE_CURRENT_BINARY_DIR}/data.txt)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME} PROPERTIES TEST_DISCOVERY_TIMEOUT 100)