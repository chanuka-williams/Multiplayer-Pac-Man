# Define game as an executable
add_executable(Pac-Man)

if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(Pac-Man PROPERTIES
            WIN32_EXECUTABLE TRUE
    )
endif()

# Automatically find all source files
file(GLOB_RECURSE GAME_SOURCES "src/*.cpp")
file(GLOB_RECURSE GAME_HEADERS "include/game/*.h" "include/game/*.hpp")

# Specify source files for the executable
target_sources(Pac-Man
    PRIVATE
        ${GAME_SOURCES}
        ${GAME_HEADERS}
)

# Add include directory for game-specific headers
target_include_directories(Pac-Man
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link engine library to game (so game can use engine functionality)
target_link_libraries(Pac-Man
    PRIVATE
        engine
)

# Sync resources after build
add_custom_command(TARGET Pac-Man POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:Pac-Man>/resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:Pac-Man>/resources"
        COMMENT "Syncing resources and profiles..."
        VERBATIM
)

