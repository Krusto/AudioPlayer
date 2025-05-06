cmake_minimum_required(VERSION 3.20)

project(AUDIO_PLAYER C CXX)

option(ENABLE_VERBOSE_LOG "Enable verbose logging" ON)
option(ENABLE_DEBUG_LOG "Enable debug log" ON)
option(ENABLE_MEMORY_DEBUG_LOG "Enable memory debug log" ON)
option(ENABLE_VENDORED "Use vendored libraries" ON)
option(SDLTTF_VENDORED "Use vendored SDL_ttf" ${ENABLE_VENDORED})

set(CMAKE_CXX_STANDARD 20)
message("Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")


if(WIN32)
    message(STATUS "${PROJECT_NAME}: Windows platform detected")
    set(LUNARA_AUDIO_PLAYER_WIN32 ON CACHE BOOL "Build Windows backend" FORCE)
    set(LUNARA_AUDIO_PLAYER_X11 OFF CACHE BOOL "Build X11 backend" FORCE)
    set(LUNARA_AUDIO_PLAYER_WAYLAND OFF CACHE BOOL "Build Wayland backend" FORCE)
else()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    execute_process(
        COMMAND bash -c "echo $XDG_SESSION_TYPE"
        OUTPUT_VARIABLE SESSION_TYPE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "${PROJECT_NAME}: Session type: ${SESSION_TYPE}")
    if(SESSION_TYPE STREQUAL "x11")
        message(STATUS "${PROJECT_NAME}: X11 detected")
        set(LUNARA_AUDIO_PLAYER_X11 ON CACHE BOOL "Build X11 backend" FORCE)
        set(LUNARA_AUDIO_PLAYER_WAYLAND OFF CACHE BOOL "Build X11 backend" FORCE)
        set(LUNARA_AUDIO_PLAYER_WIN32 OFF CACHE BOOL "Build Windows backend" FORCE)
    elseif(SESSION_TYPE STREQUAL "wayland")
        message(STATUS "${PROJECT_NAME}: Wayland detected")
        set(LUNARA_AUDIO_PLAYER_X11 OFF CACHE BOOL "Build X11 backend" FORCE)
        set(LUNARA_AUDIO_PLAYER_WAYLAND ON CACHE BOOL "Build Wayland backend" FORCE)
        set(LUNARA_AUDIO_PLAYER_WIN32 OFF CACHE BOOL "Build Windows backend" FORCE)
        set(SDL_WAYLAND ON CACHE BOOL "" FORCE)
        set(SDL_X11 OFF CACHE BOOL "" FORCE)
        set(SDL_VULKAN ON CACHE BOOL "" FORCE)
    else()
        message(STATUS "${PROJECT_NAME}: Unknown session type: ${SESSION_TYPE}")
    endif()
endif()

if(ENABLE_VENDORED)
    add_subdirectory(Vendor/SDL EXCLUDE_FROM_ALL)
    add_subdirectory(Vendor/SDL_ttf EXCLUDE_FROM_ALL)
else()
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

    find_package(SDL3 REQUIRED CONFIG COMPONENTS SDL3main)
endif()

message("${PROJECT_NAME}: Adding source files")
file(GLOB_RECURSE SANDBOX_SOURCE_FILES ./Sandbox/*.cpp ./Sandbox/*.c)
file(GLOB_RECURSE SANDBOX_HEADER_FILES ./Sandbox/*.hpp ./Sandbox/*.h)

file(GLOB_RECURSE AUDIO_PLAYER_SOURCE_FILES ./Backend/*.cpp ./Backend/*.c)
file(GLOB_RECURSE AUDIO_PLAYER_HEADER_FILES ./Backend/*.hpp ./Backend/*.h)

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

set(GLM_INCLDUE_DIR "./Vendor/glm")

add_library(AudioPlayerBackendLib STATIC ${AUDIO_PLAYER_SOURCE_FILES} ${AUDIO_PLAYER_HEADER_FILES})
set_target_properties(AudioPlayerBackendLib PROPERTIES LINKER_LANGUAGE CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(ENABLE_VERBOSE_LOG)
    add_compile_definitions(AudioPlayerBackendLib PRIVATE AUDIO_PLAYER_ENABLE_VERBOSE_LOG)
endif()

if(ENABLE_DEBUG_LOG)
    add_compile_definitions(AudioPlayerBackendLib PRIVATE AUDIO_PLAYER_ENABLE_DEBUG_LOG)
endif()

if(ENABLE_MEMORY_DEBUG_LOG)
    add_compile_definitions(AudioPlayerBackendLib PRIVATE AUDIO_PLAYER_ENABLE_MEMORY_DEBUG_LOG)
endif()

add_compile_definitions(AudioPlayerBackendLib PRIVATE $<IF:$<CONFIG:Debug>,_DEBUG,_RELEASE>)


target_compile_definitions(AudioPlayerBackendLib PRIVATE
    $<$<BOOL:${LUNARA_AUDIO_PLAYER_WIN32}>:_LUNARA_AUDIO_PLAYER_WIN32>
    $<$<BOOL:${LUNARA_AUDIO_PLAYER_X11}>:_LUNARA_AUDIO_PLAYER_X11>
    $<$<BOOL:${LUNARA_AUDIO_PLAYER_WAYLAND}>:_LUNARA_AUDIO_PLAYER_WAYLAND>
)
if(WIN32)

else()
    target_compile_definitions(AudioPlayerBackendLib PRIVATE _LINUX)
endif()

target_include_directories(AudioPlayerBackendLib PRIVATE "./Backend/src")
target_include_directories(AudioPlayerBackendLib PRIVATE "./Vendor/include")
target_include_directories(AudioPlayerBackendLib PUBLIC ${GLM_INCLDUE_DIR})

add_library(AUDIO_PLAYERInterfaceLibrary INTERFACE)

target_link_libraries(AUDIO_PLAYERInterfaceLibrary INTERFACE
    ${VULKAN_LIBRARY}
)
target_link_libraries(AudioPlayerBackendLib PRIVATE AUDIO_PLAYERInterfaceLibrary)

if(TARGET SDL3::SDL3main)
    target_link_libraries(AudioPlayerBackendLib PUBLIC SDL3::SDL3main)
endif()
target_link_libraries(AudioPlayerBackendLib PUBLIC SDL3::SDL3 SDL3_ttf::SDL3_ttf)

add_executable(Sandbox ${SANDBOX_HEADER_FILES} ${SANDBOX_SOURCE_FILES})
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/Backend/src")
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/Vendor/include")
target_include_directories(Sandbox PRIVATE "${CMAKE_SOURCE_DIR}/Sandbox")
target_link_libraries(Sandbox PRIVATE AUDIO_PLAYERInterfaceLibrary AudioPlayerBackendLib)
if(WIN32)
    add_custom_command(TARGET Sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:Sandbox> $<TARGET_FILE_DIR:Sandbox>
        COMMAND_EXPAND_LISTS
    )
endif()
function(target_add_flags target)
    if(CMAKE_CXX_COMPILE_ID MATCHES "MSVC")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(${target} PRIVATE -Wall -Wconversion -Werror -Wextra -Wpedantic)

    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(${target} PRIVATE -Wall -Wconversion -Werror -Wextra -Wpedantic)
    endif()
endfunction(target_add_flags)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Assets $<TARGET_FILE_DIR:Sandbox>/Assets DEPENDS Sandbox)

target_add_flags(AudioPlayerBackendLib)
target_add_flags(Sandbox)
