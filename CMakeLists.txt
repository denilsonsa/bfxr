cmake_minimum_required(VERSION 3.1.0)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(music)

set(BUILD_STATIC_LIBS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cpack-config.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

find_package(OpenGL REQUIRED)

# enable all warnings
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(SDL2 REQUIRED)

file(GLOB app_src_glob *.cpp;*.cc;*.h;*.inl)

include_directories(SYSTEM ${SDL2_INCLUDE_DIR})

set(app_src ${app_src_glob})
source_group("" FILES ${app_src})

include_directories(SYSTEM external/imgui)
include_directories(SYSTEM external/imgui/examples)
include_directories(SYSTEM external/glad/include)
include_directories(SYSTEM fake/)
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

add_executable(binary_to_compressed_c
  external/imgui/misc/fonts/binary_to_compressed_c.cpp)
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/font_noto_sans_display.h"
  COMMAND  binary_to_compressed_c
  ARGS ${CMAKE_CURRENT_SOURCE_DIR}/NotoSansDisplay-Regular.ttf
    NotoSansDisplay > font_noto_sans_display.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating binary font: Noto Sans Display"
)
set(app_src ${app_src} ${CMAKE_CURRENT_BINARY_DIR}/font_noto_sans_display.h)
source_group("fonts" FILES ${CMAKE_CURRENT_BINARY_DIR}/font_noto_sans_display.h)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/font_forkawesome.h"
  COMMAND  binary_to_compressed_c
  ARGS ${CMAKE_CURRENT_SOURCE_DIR}/forkawesome.ttf
    ForkAwesome > font_forkawesome.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating binary font"
  )
set(app_src ${app_src} ${CMAKE_CURRENT_BINARY_DIR}/font_forkawesome.h)
source_group("fonts" FILES ${CMAKE_CURRENT_BINARY_DIR}/font_forkawesome.h)
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

include_directories(SYSTEM external/IconFontCppHeaders/)

set(app_src ${app_src}
    # standard imgui

    external/imgui/imconfig.h
    external/imgui/imgui.cpp
    external/imgui/imgui.h
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_internal.h
    external/imgui/imgui_widgets.cpp
    external/imgui/imstb_rectpack.h
    external/imgui/imstb_textedit.h
    external/imgui/imstb_truetype.h
    # sdl binding
    external/imgui/examples/imgui_impl_sdl.h
    external/imgui/examples/imgui_impl_sdl.cpp
    # opengl binding
    external/imgui/examples/imgui_impl_opengl3.cpp
    external/imgui/examples/imgui_impl_opengl3.h
    # opengl loader
    external/glad/src/glad.c
    external/glad/include/glad/glad.h
    )

set(libs)
set(app_src ${app_src} external/nativefiledialog/src/nfd_common.c)
include_directories(SYSTEM external/nativefiledialog/src/include)
if(WIN32)
  set(app_src ${app_src} external/nativefiledialog/src/nfd_win.cpp)
  set(libs ${libs} comctl32.lib)
  message(STATUS "nfd is windows: not complete")
endif()

if(APPLE)
  set(app_src ${app_src} external/nativefiledialog/src/nfd_cocoa.m)
  set(libs ${libs} "-framework AppKit")
  message(STATUS "nfd is cocoa: not complete")
endif()

if(UNIX)
  if(NOT APPLE)
    find_package(GTK3 REQUIRED)
    set(app_src ${app_src} external/nativefiledialog/src/nfd_gtk.c)
    include_directories(SYSTEM ${GTK3_INCLUDE_DIRS})
    set(libs ${libs} ${GTK3_LIBRARIES})
    message(STATUS "nfd is gtk")
  endif()
endif()

message(STATUS "libs are ${libs}")
add_executable(bfxr WIN32 MACOSX_BUNDLE ${app_src})
target_link_libraries(bfxr
                      ${SDL2_LIBRARY}
                      ${CMAKE_DL_LIBS}
                      ${libs}
                      )

install(TARGETS bfxr DESTINATION ".")

if(APPLE)
  # install dependencies
  # install(SCRIPT macdylibbundler.cmake)

  # make apple installer look prettier
  set_target_properties(bfxr PROPERTIES MACOSX_BUNDLE_ICON_FILE "application")
  # set_target_properties(bfxr PROPERTIES MACOSX_BUNDLE_INFO_PLIST
  # "${CMAKE_CURRENT_SOURCE_DIR}/bundle-info.plist")
endif()
