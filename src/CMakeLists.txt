add_executable(comp4300 main.cpp)
target_include_directories(comp4300 PUBLIC ../inc)
target_link_libraries(comp4300 PRIVATE sfml-graphics ImGui-SFML::ImGui-SFML)

# find_package(comp4300 CONFIG REQUIRED HINTS /usr/local/lib/SFML-3.0.2)

# target_include_directories(comp4300 PRIVATE /usr/local/include/SFML-3.0.2)


# target_link_libraries(comp4300 PRIVATE /usr/local/lib/SFML-3.0.2)