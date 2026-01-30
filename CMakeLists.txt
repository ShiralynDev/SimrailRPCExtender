cmake_minimum_required(VERSION 3.20)
project(DiscordRPCExtender)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(OPENSSL_USE_STATIC_LIBS ON)
    set(BUILD_SHARED_LIBS OFF)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_SHARED_LIBS ON)
endif()

set(BUILD_EXAMPLES OFF)

set(STEAM_SDK_DIR "${CMAKE_SOURCE_DIR}/extern/steam/")
include_directories(${STEAM_SDK_DIR})
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDBUS REQUIRED sdbus-c++)

# Discord Social SDK
set(DISCORD_SDK_ROOT "${CMAKE_SOURCE_DIR}/extern/discord_social_sdk")
set(DISCORD_SDK_LIB_DIR "${DISCORD_SDK_ROOT}/lib/release")
set(DISCORD_SDK_BIN_DIR "${DISCORD_SDK_ROOT}/bin/release")
set(DISCORD_SDK_INCLUDE_DIR "${DISCORD_SDK_ROOT}/include")

# Sources
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")

add_executable(DiscordRPCExtender ${RAYLIB_ADDITIONS_SOURCES} ${SOURCES})

target_compile_options(DiscordRPCExtender PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-isystem /usr/lib/gcc/x86_64-w64-mingw32/13-win32/include/c++>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-narrowing>
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-shift-count-overflow>
)

target_include_directories(DiscordRPCExtender PRIVATE ${DISCORD_SDK_INCLUDE_DIR})

if(WIN32)
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/discord_partner_sdk.lib")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_BIN_DIR}/discord_partner_sdk.dll")
elseif(APPLE)
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.dylib")
else()
    set(DISCORD_LIB_PATH "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
    set(DISCORD_SHARED_LIB "${DISCORD_SDK_LIB_DIR}/libdiscord_partner_sdk.so")
endif()

target_link_libraries(DiscordRPCExtender PRIVATE ${DISCORD_LIB_PATH})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(DiscordRPCExtender PROPERTIES
        BUILD_WITH_INSTALL_RPATH TRUE
        INSTALL_RPATH "$ORIGIN"
        SKIP_BUILD_RPATH FALSE
    )

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

        add_custom_command(TARGET DiscordRPCExtender POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DiscordRPCExtender>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/extern/steam_bin/win64/steam_api64.dll"
        "$<TARGET_FILE_DIR:DiscordRPCExtender>/steam_api64.dll"
        VERBATIM)

        add_custom_command(TARGET DiscordRPCExtender POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DISCORD_LIB_PATH}"
        "$<TARGET_FILE_DIR:DiscordRPCExtender>/discord_partner_sdk.dll"
        VERBATIM)

    else()

        add_custom_command(TARGET DiscordRPCExtender POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DiscordRPCExtender>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_LIST_DIR}/extern/steam_bin/linux64/libsteam_api.so"
        "$<TARGET_FILE_DIR:DiscordRPCExtender>/libsteam_api.so"
        VERBATIM)

        add_custom_command(TARGET DiscordRPCExtender POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DISCORD_LIB_PATH}"
        "$<TARGET_FILE_DIR:DiscordRPCExtender>/libdiscord_partner_sdk.so"
        VERBATIM)

    endif()

    add_custom_command(TARGET DiscordRPCExtender POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_LIST_DIR}/steam_appid.txt"
    "$<TARGET_FILE_DIR:DiscordRPCExtender>/steam_appid.txt"
    VERBATIM)

endif()

add_library(steam_api SHARED IMPORTED)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
set_target_properties(steam_api PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/extern/steam_bin/win64/steam_api64.dll"
    IMPORTED_IMPLIB "${CMAKE_CURRENT_LIST_DIR}/extern/steam_bin/win64/steam_api64.lib")
else()
set_target_properties(steam_api PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/extern/steam_bin/linux64/libsteam_api.so")
endif()

target_link_libraries(DiscordRPCExtender PRIVATE steam_api)

# optimization
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # Add optimization flags
    target_compile_options(DiscordRPCExtender PRIVATE -O3)
    
    # strip symbols to reduce binary size
    if (NOT WIN32)
        target_link_options(DiscordRPCExtender PRIVATE -s)
    endif()
endif()