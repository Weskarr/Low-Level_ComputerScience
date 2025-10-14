# Add all source files
file(GLOB_RECURSE SRC_FILES
    "*.cpp"
)

# For showing headers
file(GLOB_RECURSE HEADER_FILES
    "*.h"
)

# Create the executable
add_executable(low-level_setup
    ${SRC_FILES}
    ${HEADER_FILES}
)

# Link libraries
target_link_libraries(low-level_setup
    PUBLIC
        ImGui-SFML::ImGui-SFML
        cppzmq
        libzmq
)

# So it can easily be run
add_custom_target(run
    COMMAND $<TARGET_FILE:low-level_setup>
    DEPENDS low-level_setup
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
