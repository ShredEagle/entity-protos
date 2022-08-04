set(TARGET_NAME bawls)

set(${TARGET_NAME}_HEADERS
    Bawls.h
    Timer.h

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

add_executable(${TARGET_NAME}
               ${${TARGET_NAME}_HEADERS}
               ${${TARGET_NAME}_SOURCES}
)


##
## Dependencies
##

find_package(Entity CONFIG REQUIRED COMPONENTS entity)
find_package(Graphics CONFIG REQUIRED COMPONENTS graphics)

target_link_libraries(${TARGET_NAME}
    PRIVATE
        ad::entity
        ad::graphics
)

set_target_properties(${TARGET_NAME} PROPERTIES
                      VERSION "${${PROJECT_NAME}_VERSION}"
)

install(TARGETS ${TARGET_NAME})
