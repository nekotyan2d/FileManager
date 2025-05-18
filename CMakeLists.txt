cmake_minimum_required(VERSION 3.21)
project(FileManager LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(Qt6_DIR "C:/Qt/6.9.0/mingw_64/lib/cmake/Qt6")

find_package(Qt6 6.9.0 REQUIRED COMPONENTS Core Widgets Svg Concurrent)

qt_standard_project_setup()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set (APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/Resources/app.rc)
endif()

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
    FilePreviewWidget.h FilePreviewWidget.cpp FilePreviewWidget.ui
    ExtendedTreeView.h ExtendedTreeView.cpp
    ${APP_ICON}
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
        Resources/arrow-down.svg
        Resources/arrow-right.svg
        Styles/app.qss
)

target_link_libraries(FileManager
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        Qt6::Svg
        Qt6::Concurrent
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
