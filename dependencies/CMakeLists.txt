include(FetchContent)

# Add SFML? Or use 'installed' libs as is now?

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui
  GIT_TAG v1.92.6
)

FetchContent_MakeAvailable(imgui)