include(FetchContent)

set(SFML_VERSION 3.0.0)
set(IMGUI_VERSION 1.91.1)

option(LINK_DEPS_STATIC CACHE ON)
if(LINK_DEPS_STATIC)
    set(BUILD_SHARED_LIBS OFF)
else()
    set(BUILD_SHARED_LIBS ON)
endif()

# SFML
FetchContent_Declare(
 SFML
 URL "https://github.com/SFML/SFML/archive/${SFML_VERSION}.zip"
)
option(SFML_BUILD_AUDIO OFF)
option(SFML_BUILD_NETWORK OFF)
FetchContent_MakeAvailable(sfml)

# ImGui
FetchContent_Declare(
 imgui
 URL "https://github.com/ocornut/imgui/archive/v${IMGUI_VERSION}.zip"
)
FetchContent_MakeAvailable(imgui)

# ImGui-SFML
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
  GIT_TAG        v3.0
)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON CACHE BOOL "Enable imgui demo" FORCE)
FetchContent_MakeAvailable(imgui-sfml)

# ZeroMQ
FetchContent_Declare(
    libzmq
    GIT_REPOSITORY https://github.com/zeromq/libzmq.git
    GIT_TAG        Master
)
FetchContent_MakeAvailable(libzmq)

FetchContent_Declare(
    cppzmq
    GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
    GIT_TAG        Master
)
FetchContent_MakeAvailable(cppzmq)
