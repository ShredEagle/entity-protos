#add_subdirectory(libs/entityprotos/entityprotos)

add_subdirectory(apps/bawls/bawls)

option (BUILD_tests "Build 'tests' application" true)
if(BUILD_tests)
    # No tests atm
endif()
