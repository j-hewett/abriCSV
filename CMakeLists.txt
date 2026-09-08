cmake_minimum_required(VERSION 3.19)
project(abriCSV LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(abriCSV
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/dsvtablemodel.h src/dsvtablemodel.cpp
    src/dsvindex.h src/dsvindex.cpp
)

option(USE_SYSTEM_SQLITE "Link against system libsqlite3 instead of vendored amalgamation" OFF)

if(USE_SYSTEM_SQLITE)
    find_package(SQLite3 REQUIRED)
    add_library(SQLite::SQLite3 ALIAS SQLite::SQLite3)
else()
    add_subdirectory(external/sqlite3)
endif()

target_link_libraries(abriCSV
PRIVATE
    Qt::Core
    Qt::Widgets
    SQLite::SQLite3
)

include(GNUInstallDirs)

install(TARGETS abriCSV
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET abriCSV
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
