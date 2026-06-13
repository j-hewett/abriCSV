cmake_minimum_required(VERSION 3.19)
project(abriCSV LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Concurrent)

qt_standard_project_setup()

qt_add_executable(abriCSV
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    csvtablemodel.h csvtablemodel.cpp
    test.csv
)

target_link_libraries(abriCSV
    PRIVATE
        Qt::Core
        Qt::Widgets
)

configure_file(${CMAKE_SOURCE_DIR}/test.csv ${CMAKE_BINARY_DIR}/test.csv COPYONLY)


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
