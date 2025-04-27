cmake_minimum_required(VERSION 3.26)
project(Prog4-Engine)

include(FetchContent)

if(EMSCRIPTEN)
  message("Using Emscripten!")
endif()

if(EMSCRIPTEN)
  set(TARGET_NAME minigin_web)
  set(TARGET_PARAM )
else()
  set(TARGET_NAME Prog4-Engine)
  set(TARGET_PARAM WIN32)
endif()

FetchContent_Declare(
      ImGui
      GIT_REPOSITORY https://github.com/ocornut/imgui
      GIT_TAG        v1.91.7-docking
      GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ImGui)

FetchContent_Declare(
      ImPlot
      GIT_REPOSITORY https://github.com/epezent/implot
      GIT_TAG        v0.16
      GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(ImPlot)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0 # <-- or whatever version you prefer
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(spdlog)
set(SPDLOG_INCLUDE_DIR ${spdlog_SOURCE_DIR}/include)

# Need to add this to avoid deprecation warnings
add_compile_definitions(_SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING)


set(IMGUI_SRC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp")

set(IMPLOT_SRC
    "${implot_SOURCE_DIR}/implot.cpp"
    "${implot_SOURCE_DIR}/implot_items.cpp")

set(ENGINE_SOURCE
    "Minigin/Font.cpp"
    "Minigin/GameObject.cpp"
    "Minigin/InputManager.cpp"
    "Minigin/Minigin.cpp"
    "Minigin/Renderer.cpp"
    "Minigin/ResourceManager.cpp"
    "Minigin/Scene.cpp"
    "Minigin/SceneManager.cpp"
    "Minigin/Texture2D.cpp"
    "Minigin/Transform.cpp"
    "Minigin/Components/Component.cpp"
    "Minigin/Components/FpsComponent.cpp"
    "Minigin/Components/TextComponent.cpp"
    "Minigin/Components/TextureComponent.cpp" 
    "Minigin/DaeTime.cpp"
    "Minigin/ImGuiRenderer.cpp" 
    "Minigin/Input/Command.cpp" 
    "Minigin/Input/XInputGamepad.cpp"
    "Minigin/Input/Gamepad.cpp" 
    "Minigin/Input/Observer.cpp"
    "Minigin/Audio/Servicelocator.cpp"
    "Minigin/Audio/Soundsystem.cpp")

# list source files here
add_library(${TARGET_NAME} STATIC
    ${ENGINE_SOURCE}
    ${IMGUI_SRC} 
    ${IMPLOT_SRC})

target_include_directories(${TARGET_NAME} PUBLIC 
    ${implot_SOURCE_DIR} 
    ${imgui_SOURCE_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}/Minigin 
    ${CMAKE_CURRENT_SOURCE_DIR}/Minigin/Input 
    ${CMAKE_CURRENT_SOURCE_DIR}/Minigin/Components 
    ${CMAKE_CURRENT_SOURCE_DIR}/Minigin/Components)

# enable c++20 features
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)
# set minigin as the default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})

# add glm
FetchContent_Declare(
  glm
  URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
  DOWNLOAD_NO_PROGRESS ON
  DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
  FetchContent_MakeAvailable(glm)
endif()

# add SDL2
  FetchContent_Declare(
    SDL2
    URL https://www.libsdl.org/release/SDL2-devel-2.30.10-VC.zip 
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads 
  )
  FetchContent_GetProperties(SDL2)
  if(NOT SDL2_POPULATED)
    FetchContent_MakeAvailable(SDL2)
    set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
    set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
  else()
    set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
    set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
  endif()
  endif()

  # add SDL2_image
  FetchContent_Declare(
    SDL2_image
    URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.3-VC.zip 
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_GetProperties(SDL2_image)
  if(NOT SDL2_image_POPULATED)
    FetchContent_MakeAvailable(SDL2_image)
    set(SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include)
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib")
    set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll")
  else()
    set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib")
    set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll")
  endif()
  endif()

  # add SDL2_ttf
  FetchContent_Declare(
    SDL2_ttf
    URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-VC.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_GetProperties(SDL2_ttf)
  if(NOT sdl2_ttf_POPULATED)
    FetchContent_MakeAvailable(SDL2_ttf)
    set(SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include)
  if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib)
    set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll")
  else()
    set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib)
    set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll")
  endif()
  endif()

  # add SDL2_mixer
  FetchContent_Declare(
      SDL2_mixer
      URL https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.8.0-VC.zip
      DOWNLOAD_NO_PROGRESS ON
      DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
  )
  FetchContent_GetProperties(SDL2_mixer)
  if(NOT sdl2_mixer_POPULATED)
      FetchContent_MakeAvailable(SDL2_mixer)
      set(SDL2_MIXER_INCLUDE_DIR ${sdl2_mixer_SOURCE_DIR}/include)
      if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
      set(SDL2_MIXER_LIBRARIES ${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.lib)
      set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.dll")
      else()
      set(SDL2_MIXER_LIBRARIES ${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.lib)
      set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.dll")
      endif()
  endif()


  # add logger


  find_package(VLD CONFIG)

# set all include directories
target_include_directories(${TARGET_NAME} PUBLIC
    ${VLD_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR} 
    ${SDL2_IMAGE_INCLUDE_DIR} 
    ${SDL2_TTF_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${SPDLOG_INCLUDE_DIR}
)
  
# set libraries to link with
target_link_libraries(${TARGET_NAME} PUBLIC
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES} 
    ${SDL2_MIXER_LIBRARIES}
    ${VLD_LIBRARY}
    XInput
    spdlog::spdlog
)

target_include_directories(${TARGET_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/Minigin/
  ${glm_SOURCE_DIR}
)

# compile at max warning level + treat warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")

# add post build commands
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)

set(SDL2_LIBRARIES ${SDL2_LIBRARIES} PARENT_SCOPE)
set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARIES} PARENT_SCOPE)
set(SDL2_TTF_LIBRARIES ${SDL2_TTF_LIBRARIES} PARENT_SCOPE)
set(SDL2_MIXER_LIBRARIES ${SDL2_MIXER_LIBRARIES} PARENT_SCOPE)
set(VLD_LIBRARY ${VLD_LIBRARY} PARENT_SCOPE)

set(SDL2_LIBRARY_DLL ${SDL2_LIBRARY_DLL} PARENT_SCOPE)
set(SDL2_IMAGE_LIBRARY_DLL ${SDL2_IMAGE_LIBRARY_DLL} PARENT_SCOPE)
set(SDL2_TTF_LIBRARY_DLL ${SDL2_TTF_LIBRARY_DLL} PARENT_SCOPE)
set(SDL2_MIXER_LIBRARY_DLL ${SDL2_MIXER_LIBRARY_DLL} PARENT_SCOPE)

set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIR} PARENT_SCOPE)
set(SDL2_IMAGE_INCLUDE_DIR ${SDL2_IMAGE_INCLUDE_DIR} PARENT_SCOPE)
set(SDL2_TTF_INCLUDE_DIR ${SDL2_TTF_INCLUDE_DIR} PARENT_SCOPE)
set(SDL2_MIXER_INCLUDE_DIR ${SDL2_MIXER_INCLUDE_DIR} PARENT_SCOPE)
set(VLD_INCLUDE_DIR ${VLD_INCLUDE_DIR} PARENT_SCOPE)

set(ENGINE_REQ_LIBRARIES ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${VLD_LIBRARY} PARENT_SCOPE)
set(ENGINE_REQ_INCLUDE_DIRS ${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${VLD_INCLUDE_DIR} PARENT_SCOPE)
set(ENGINE_REQ_DLL ${SDL2_LIBRARY_DLL} ${SDL2_IMAGE_LIBRARY_DLL} ${SDL2_TTF_LIBRARY_DLL} ${SDL2_MIXER_LIBRARY_DLL} PARENT_SCOPE)