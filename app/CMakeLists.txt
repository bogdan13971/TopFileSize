project(FileSizeExplorer)

add_executable(${PROJECT_NAME} "main.cpp")

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} 
	FileSizeExplorer_Lib
)