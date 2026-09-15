cmake_minimum_required(VERSION 3.19)
project(abriCSV LANGUAGES CXX)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.9 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(sources
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/dsvtablemodel.h src/dsvtablemodel.cpp
    src/dsvindex.h src/dsvindex.cpp
    resources/res.qrc
)

qt_add_executable(abriCSV ${sources})

target_link_libraries(abriCSV
    PRIVATE
        Qt::Core
        Qt::Widgets
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
