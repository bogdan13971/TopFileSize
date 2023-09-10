project(FileSizeExplorer_Lib)

set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS filesystem thread)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.hpp" "*.cpp")

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${Boost_INCLUDE_DIRS}
	PUBLIC ${CMAKE_SOURCE_DIR}/../ConsoleUI/include
)

target_link_libraries(${PROJECT_NAME} 
	Boost::filesystem
	Boost::thread
	${CMAKE_SOURCE_DIR}/../ConsoleUI/lib/Debug/ConsoleUI.lib
)