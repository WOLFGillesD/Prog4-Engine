cmake_minimum_required(VERSION 3.26)
project(Prog4-Game)

include(FetchContent)


set(TARGET_NAME Prog4-Game)
set(TARGET_PARAM WIN32)

set(GAME_SOURCE     
    "src/Main.cpp"
    "src/Components/Characters/HealthComponent.cpp"
    "src/Components/RotatorComponent.cpp"
    "src/Components/Score/ScoreComponent.cpp"
    "src/Components/GridComponent.cpp" 
    "src/Components/States.h"
    "src/Components/Characters/PlayerComponent.cpp"
    "src/Components/Characters/NPCComponents.cpp"
    "src/Scenes/SceneLoader.h" 
    "src/Components/UI/UIControllerComponent.cpp" 
    "src/Components/Score/GameManager.cpp")

set(GAME_INCL
    "src/Components"
    "src/Scenes"
    "src/Components/Characters"
    "src/Components/Score"
    "src/Components/UI")

# list source files here
add_executable(
    ${TARGET_NAME} 
    ${TARGET_PARAM}
    ${GAME_SOURCE}
    )

target_include_directories(${TARGET_NAME} PRIVATE ${implot_SOURCE_DIR} ${imgui_SOURCE_DIR} ${GAME_INCL})

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

  find_package(VLD CONFIG)

  # set all include directories
  target_include_directories(${TARGET_NAME} PRIVATE
   ${ENGINE_REQ_INCLUDE_DIRS})
  
  # set libraries to link with
  target_link_libraries(${TARGET_NAME} PRIVATE 
    ${ENGINE_REQ_LIBRARIES}
    Prog4-Engine
    XInput
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
  add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Data" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Data"
  )
