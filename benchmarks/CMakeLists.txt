file(GLOB_RECURSE BENCH_SRCS CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach (src IN LISTS BENCH_SRCS)
    get_filename_component(name ${src} NAME_WE)
    add_executable(${name} ${src})
    target_link_libraries(${name}
            PRIVATE
            linalg_lib
            benchmark::benchmark
    )
    add_test(NAME ${name} COMMAND ${name} --benchmark_min_time=0.01)
endforeach ()
