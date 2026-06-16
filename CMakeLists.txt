cmake_minimum_required(VERSION 3.16)
project(MemoryGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    set(QT_LIBRARIES Qt5::Widgets)
    set(QT_VERSION "5")
else()
    set(QT_LIBRARIES Qt6::Widgets)
    set(QT_VERSION "6")
endif()

message(STATUS "Using Qt${QT_VERSION}")

set(SOURCES
    src/main.cpp
    src/gameboard.cpp
    src/mainmenu.cpp
)

set(HEADERS
    src/gameboard.h
    src/mainmenu.h
)

add_executable(MemoryGame WIN32 ${SOURCES} ${HEADERS})
target_link_libraries(MemoryGame ${QT_LIBRARIES})

if(WIN32 AND Qt6_FOUND)
    add_custom_command(TARGET MemoryGame POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_RUNTIME_DLLS:MemoryGame>"
            "$<TARGET_FILE_DIR:MemoryGame>"
        COMMAND_EXPAND_LISTS
    )
endif()

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_executable(test_memory_game
    test_memory_game.cpp
    src/gameboard.cpp
    src/mainmenu.cpp
)

target_include_directories(test_memory_game PRIVATE src)
target_link_libraries(test_memory_game
    ${QT_LIBRARIES}
    gtest_main
)

add_test(NAME MemoryGameTest COMMAND test_memory_game)
