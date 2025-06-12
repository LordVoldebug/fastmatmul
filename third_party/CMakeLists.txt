include(FetchContent)

# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)
# set(FETCHCONTENT_FULLY_DISCONNECTED ON)

# set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# CMP0135: Prevents FetchContent_MakeAvailable from reconfiguring dependencies
# that were already added manually. Ensures consistent behavior when dependencies
# are pre-initialized.
cmake_policy(SET CMP0135 NEW)

# --------------------------------------------------------------------

# Libraries

# --------------------------------------------------------------------
# Gtest

FetchContent_Declare(
        gtest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

# --------------------------------------------------------------------
# Gbench
FetchContent_Declare(
        gbench
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.9.4
)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gbench)
