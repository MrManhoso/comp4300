include(FetchContent)

# TODO Or use 'installed' under /usr/local/...?
FetchContent_Declare(
  sfml
  GIT_REPOSITORY "https://github.com/SFML/SFML"
  GIT_TAG 3.0.2 # TODO replace with hash (it is faster)
)

add_subdirectory(sfml)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.91.1 # 1.92 not supported by imgui-sfml 3.0? TODO replace with hash (it is faster)
)

FetchContent_MakeAvailable(imgui)

# ImGui-SFML
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml
  GIT_TAG v3.0 # TODO replace with hash (it is faster)
)
add_subdirectory(imgui-sfml)