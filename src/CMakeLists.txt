add_subdirectory(libs/utils/utils)

add_subdirectory(apps/bawls/bawls)
add_subdirectory(apps/gameloop-decouple/gameloop-decouple)
add_subdirectory(apps/stutter-investigate/stutter-investigate)

option (BUILD_tests "Build 'tests' application" true)
if(BUILD_tests)
    # No tests atm
endif()
