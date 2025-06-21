file(GLOB_RECURSE TEST_SRCS CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach (src IN LISTS TEST_SRCS)
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
    target_link_libraries(${name}
            PRIVATE
            linalg_lib
            GTest::gtest_main
    )
    target_include_directories(${name}
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    include(GoogleTest)
    gtest_discover_tests(${name})
endforeach ()
