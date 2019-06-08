project(ThreadSafeContainer)

include_directories(
        ${PROJECT_SOURCE_DIR}
)

aux_source_directory(${PROJECT_SOURCE_DIR} TSC_SRCS)

set(LIBRARY_OUTPUT_PATH ${PROJ_LIB_OUTPUT_PATH})

add_library(${PROJECT_NAME} SHARED ${TSC_SRCS} ThreadSafeStack.hpp)

