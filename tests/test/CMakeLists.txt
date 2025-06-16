file(GLOB TEST_SRCS "*.cpp")

foreach (src IN LISTS TEST_SRCS)
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
    target_link_libraries(${name}
            PRIVATE
            linalg_lib
            GTest::gtest_main
    )
    include(GoogleTest)
    gtest_discover_tests(${name})
endforeach ()
