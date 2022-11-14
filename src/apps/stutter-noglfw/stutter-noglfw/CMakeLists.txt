set(TARGET_NAME stutter-noglfw)

set(${TARGET_NAME}_HEADERS
    Timing.h
    WindowAndContext.h

    wglext.h
)

set(${TARGET_NAME}_SOURCES
    main.cpp
)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}
             FILES ${${TARGET_NAME}_HEADERS} ${${TARGET_NAME}_SOURCES}
)

add_executable(${TARGET_NAME}
               ${${TARGET_NAME}_HEADERS}
               ${${TARGET_NAME}_SOURCES}
)


##
## Dependencies
##

find_package(Entity CONFIG REQUIRED COMPONENTS entity)
find_package(Graphics CONFIG REQUIRED COMPONENTS graphics imguiui)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        ad::entityprotos::utils

        ad::entity
        ad::math
)

# NVTX header
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_include_directories(${TARGET_NAME}
      PRIVATE "C:/Program Files/NVIDIA Corporation/Nsight Systems 2022.3.4/target-windows-x64/nvtx/include"
    )
endif()

set_target_properties(${TARGET_NAME} PROPERTIES
                      VERSION "${${PROJECT_NAME}_VERSION}"
)

install(TARGETS ${TARGET_NAME})
