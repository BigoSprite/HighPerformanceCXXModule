project(test)

include_directories(./)

aux_source_directory(../thead_safe_container TSC_SRCS)

add_executable(${PROJECT_NAME} ../main.cpp ${TSC_SRCS} )