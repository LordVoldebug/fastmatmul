add_library(linalg_lib INTERFACE)

file(GLOB_RECURSE LINALG_HEADERS
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.h
)

target_sources(linalg_lib
        INTERFACE
        ${LINALG_HEADERS}
)

target_include_directories(linalg_lib
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)
