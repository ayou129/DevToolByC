cmake_minimum_required(VERSION 3.5)

project(DevTool VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加vcpkg工具链文件
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows")
# ${CMAKE_PREFIX_PATH};
find_package(Vulkan REQUIRED)

# set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

set(QT_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6")
set(Qt6_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6")
set(Qt6CoreTools_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6CoreTools")
set(Qt6GuiTools_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6GuiTools")

# 下面的不用set 可以通过ADD Path=C:\Qt\6.7.1\mingw_64\bin
# set(Qt6Gui_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6Gui")
# set(Qt6WidgetsTools_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6WidgetsTools")
# set(QT_QMAKE_EXECUTABLE "C:/Qt/6.7.1/mingw_64/bin/qmake.exe")
# set(Qt6EntryPointPrivate_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6EntryPointPrivate")
# set(Qt6LinguistTools_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6LinguistTools")
# set(Qt6Widgets_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6Widgets")
# set(Qt6ZlibPrivate_DIR "C:/Qt/6.7.1/mingw_64/lib/cmake/Qt6ZlibPrivate")

# set(libssh_DIR "C:/vcpkg/installed/x64-windows/share/libssh")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
find_package(libssh CONFIG REQUIRED)
add_custom_target(
  install_devtool
  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_install.cmake
)
install(FILES
  lib/vulkan-1.dll
  DESTINATION ${CMAKE_BINARY_DIR}/lib
)

# fonts
# 复制字体文件到构建目录
file(COPY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_BINARY_DIR}/fonts)

# 安装字体文件到安装目录
install(DIRECTORY ${CMAKE_SOURCE_DIR}/fonts DESTINATION ${CMAKE_INSTALL_PREFIX}/fonts)


# install(DIRECTORY fonts DESTINATION ${CMAKE_BINARY_DIR})

set(TS_FILES DevTool_yue_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        TerminalWidget.cpp
        AnsiParser.cpp
        parse/ascii.hpp
        parse/stateMachine.cpp
        parse/stateMachine.hpp
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(DevTool
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DevTool APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(DevTool SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(DevTool
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(DevTool PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ssh Vulkan::Vulkan)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.DevTool)
endif()
set_target_properties(DevTool PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS DevTool
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(DevTool)
endif()
