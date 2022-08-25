set(TARGET_NAME stutter-investigate)

set(${TARGET_NAME}_HEADERS
    Bawls.h
    Metrics.h
    Timing.h

    ComponentGeometry.h
    ComponentVelocity.h

    SystemCollide.h
    SystemMove.h
    SystemRender.h
)

set(${TARGET_NAME}_SOURCES
    Bawls.cpp
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
        ad::graphics
        ad::imguiui
)

set_target_properties(${TARGET_NAME} PROPERTIES
                      VERSION "${${PROJECT_NAME}_VERSION}"
)

install(TARGETS ${TARGET_NAME})
