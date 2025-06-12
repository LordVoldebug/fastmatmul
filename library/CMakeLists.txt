add_library(fastmatmul INTERFACE)

target_include_directories(fastmatmul
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${vectorclass_SOURCE_DIR}
)
