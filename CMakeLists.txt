cmake_minimum_required(VERSION 3.21)
project(basson
    VERSION 1.0.0
    DESCRIPTION "My reusable C library"
    LANGUAGES C)

# option(MYLIBRARY_BUILD_TESTS    "Build tests"    OFF)
# option(MYLIBRARY_BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_SHARED_LIBS        "Build as shared library" OFF)

add_library(basson src/terminal.c)
add_library(basson::basson ALIAS basson)

target_include_directories(basson
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_compile_features(basson PUBLIC c_std_90)

#if(MYLIBRARY_BUILD_TESTS)
#    enable_testing()
#    add_subdirectory(tests)
#endif()

#if(MYLIBRARY_BUILD_EXAMPLES)
#    add_subdirectory(examples)
#endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS basson
    EXPORT  bassonTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT bassonTargets
    FILE        bassonTargets.cmake
    NAMESPACE   basson::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/basson)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/bassonConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/bassonConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/basson)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/bassonConfigVersion.cmake
    VERSION       ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/bassonConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/bassonConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/basson)
