cmake_minimum_required(VERSION 3.19)
project(FileManager LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Svg)

qt_standard_project_setup()

qt_add_executable(FileManager
    WIN32 MACOSX_BUNDLE
    main.cpp
    FileManager.cpp
    FileManager.h
    FileManager.ui
    FileModel.h FileModel.cpp
    DirTreeModel.h DirTreeModel.cpp
    FileModelDelegate.h FileModelDelegate.cpp
    DirTreeDelegate.h DirTreeDelegate.cpp
)

qt_add_resources(FileManager "app_resources"
    PREFIX "/FileManager"
    FILES
        Resources/copy.svg
        Resources/delete.svg
        Resources/icon.png
        Resources/move.svg
        Resources/open.svg
        Resources/refresh.svg
        Styles/app.qss
)

target_link_libraries(FileManager
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Svg
)

include(GNUInstallDirs)

install(TARGETS FileManager
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET FileManager
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
