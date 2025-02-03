# -----------------------------------------------------------------------------
# @brief  : Root cmake file.
# @author : Enrico Fraccaroli
# -----------------------------------------------------------------------------

# Set the minimum CMake version, the project name and default build type.
cmake_minimum_required(VERSION 3.1...3.18)

# Set the project name.
project(ordered_map CXX)

# Set the default build type to Debug.
if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)
endif()

# -----------------------------------------------------------------------------
# OPTIONS
# -----------------------------------------------------------------------------

option(STRICT_WARNINGS "Enable strict compiler warnings" ON)
option(WARNINGS_AS_ERRORS "Treat all warnings as errors" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)
option(BUILD_TESTS "Build tests" OFF)

# -----------------------------------------------------------------------------
# ENABLE FETCH CONTENT
# -----------------------------------------------------------------------------

# We need this in order to import external projects.
include(FetchContent)
# Hide fetchcontent variables.
mark_as_advanced(FORCE
    FETCHCONTENT_QUIET
    FETCHCONTENT_BASE_DIR
    FETCHCONTENT_FULLY_DISCONNECTED
    FETCHCONTENT_UPDATES_DISCONNECTED
)

# -----------------------------------------------------------------------------
# DEPENDENCY (SYSTEM LIBRARIES)
# -----------------------------------------------------------------------------

find_package(Doxygen)

find_program(CLANG_TIDY_EXE NAMES clang-tidy)

# -----------------------------------------------------------------------------
# LIBRARY
# -----------------------------------------------------------------------------

# Add the C++ Library.
add_library(ordered_map INTERFACE)
add_library(ordered_map::ordered_map ALIAS ordered_map)
# Include header directories, and link libraries.
target_include_directories(ordered_map INTERFACE ${PROJECT_SOURCE_DIR}/include)
# Set the library to use c++-11
target_compile_features(ordered_map INTERFACE cxx_std_11)

# =====================================
# COMPILATION FLAGS
# =====================================

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # Disable warnings for MSVC-specific "safe" functions like strcpy_s, etc.,
    # which are not portable and may clutter warning logs.
    target_compile_definitions(ordered_map INTERFACE _CRT_SECURE_NO_WARNINGS)

    # Disable warning C4702: unreachable code.
    add_compile_options(/wd4702)

    if(WARNINGS_AS_ERRORS)
        # Treat all warnings as errors to enforce stricter code quality.
        target_compile_options(ordered_map INTERFACE /WX)
    endif()

    if(STRICT_WARNINGS)
        # Enable external header management to suppress warnings in system and
        # external headers, making it easier to focus on project-specific issues.
        target_compile_options(ordered_map INTERFACE /experimental:external)
        target_compile_options(ordered_map INTERFACE /external:I ${CMAKE_BINARY_DIR})
        target_compile_options(ordered_map INTERFACE /external:anglebrackets)
        target_compile_options(ordered_map INTERFACE /external:W0)

        # Use a high warning level to catch as many potential issues as possible.
        target_compile_options(ordered_map INTERFACE /W4)

        # Enforce standards-compliant behavior to avoid relying on MSVC-specific extensions.
        target_compile_options(ordered_map INTERFACE /permissive-)
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(WARNINGS_AS_ERRORS)
        # Treat all warnings as errors to enforce stricter code quality.
        target_compile_options(ordered_map INTERFACE -Werror)
    endif()

    if(STRICT_WARNINGS)
        # Enable a broad set of warnings to catch common and subtle issues:
        target_compile_options(ordered_map INTERFACE
            -Wall                # Enable most general-purpose warnings.
            -Wextra              # Enable extra warnings not included in -Wall.
            -Wconversion         # Warn about implicit type conversions that may lose data.
            -pedantic            # Enforce strict compliance with the C++ standard.
            -Wshadow             # Warn about variable shadowing, which can cause subtle bugs.
            -Wnon-virtual-dtor   # Warn when a class with virtual functions lacks a virtual destructor.
            -Wnull-dereference   # Warn about potential null pointer dereferences.
            -Wformat=2           # Enable strict checks for printf/scanf format strings.
            -Woverloaded-virtual # Warn when a derived class function hides a base class virtual function.
            -Wfloat-equal        # Warn about direct comparisons of floating-point values, which can be imprecise.
        )

    endif()
endif()

# -----------------------------------------------------------------------------
# EXAMPLES
# -----------------------------------------------------------------------------

if(BUILD_EXAMPLES)
    # Add the example.
    add_executable(ordered_map_example ${PROJECT_SOURCE_DIR}/examples/example.cpp)
    # Set the linked libraries.
    target_link_libraries(ordered_map_example PUBLIC ordered_map)
endif()

# -----------------------------------------------------------------------------
# TESTS
# -----------------------------------------------------------------------------

if(BUILD_TESTS)
    # CMake has support for adding tests to a project.
    enable_testing()
    # Add the test.
    add_executable(ordered_map_test ${PROJECT_SOURCE_DIR}/tests/test.cpp)
    add_test(NAME ordered_map_test_run_0 COMMAND ordered_map_test 0)
    add_test(NAME ordered_map_test_run_1 COMMAND ordered_map_test 1)
    add_test(NAME ordered_map_test_run_2 COMMAND ordered_map_test 2)
    add_test(NAME ordered_map_test_run_3 COMMAND ordered_map_test 3)
    add_test(NAME ordered_map_test_run_4 COMMAND ordered_map_test 4)
    # Liking for the test.
    target_link_libraries(ordered_map_test ordered_map)
endif()

# -----------------------------------------------------------------------------
# CODE ANALYSIS
# -----------------------------------------------------------------------------

if(CLANG_TIDY_EXE)
    file(GLOB_RECURSE PROJECT_HEADERS "${PROJECT_SOURCE_DIR}/include/**/*.hpp")
    file(GLOB_RECURSE PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/src/**/*.cpp")
    add_custom_target(
        ordered_map_clang_tidy
        COMMAND ${CLANG_TIDY_EXE}
        --p=${CMAKE_BINARY_DIR}
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        COMMENT "Running clang-tidy"
        VERBATIM
    )
    add_custom_target(
        ordered_map_clang_tidy_fix
        COMMAND ${CLANG_TIDY_EXE}
        --fix
        --p=${CMAKE_BINARY_DIR}
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        COMMENT "Running clang-tidy"
        VERBATIM
    )
endif()

# -----------------------------------------------------------------------------
# DOCUMENTATION
# -----------------------------------------------------------------------------

if(DOXYGEN_FOUND)
    # FetchContent: Doxygen Awesome CSS
    FetchContent_Declare(doxygenawesome
        GIT_REPOSITORY https://github.com/jothepro/doxygen-awesome-css
        GIT_TAG main
    )
    FetchContent_MakeAvailable(doxygenawesome)

    # Hide FetchContent variables to avoid clutter in ccmake.
    mark_as_advanced(FORCE
        FETCHCONTENT_UPDATES_DISCONNECTED_DOXYGENAWESOME
        FETCHCONTENT_SOURCE_DIR_DOXYGENAWESOME
    )

    # Read the file with the version.
    file(READ ${PROJECT_SOURCE_DIR}/include/ordered_map/ordered_map.hpp version_file)
    # Extract the version.
    string(REGEX MATCH "ORDERED_MAP_MAJOR_VERSION ([0-9]*)" _ ${version_file})
    set(PROJECT_MAJOR_VERSION ${CMAKE_MATCH_1})
    string(REGEX MATCH "ORDERED_MAP_MINOR_VERSION ([0-9]*)" _ ${version_file})
    set(PROJECT_MINOR_VERSION ${CMAKE_MATCH_1})
    string(REGEX MATCH "ORDERED_MAP_MICRO_VERSION ([0-9]*)" _ ${version_file})
    set(PROJECT_MICRO_VERSION ${CMAKE_MATCH_1})

    # Customization: Doxygen Configuration
    set(DOXYGEN_WARN_FORMAT "$file:$line:1: $text")
    set(DOXYGEN_PROJECT_NAME "Ordered Map")
    set(DOXYGEN_PROJECT_BRIEF "An ordered map for C++")
    set(DOXYGEN_PROJECT_NUMBER "${PROJECT_MAJOR_VERSION}.${PROJECT_MINOR_VERSION}.${PROJECT_MICRO_VERSION}")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE ${PROJECT_SOURCE_DIR}/README.md)
    set(DOXYGEN_SHOW_INCLUDE_FILES NO)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_GENERATE_LATEX NO) # Disable LaTeX if not needed
    set(DOXYGEN_GENERATE_MAN NO)   # Disable man pages if not needed

    # Styling and UX enhancements using Doxygen Awesome
    set(DOXYGEN_HTML_HEADER ${doxygenawesome_SOURCE_DIR}/doxygen-custom/header.html)
    set(DOXYGEN_HTML_EXTRA_STYLESHEET ${doxygenawesome_SOURCE_DIR}/doxygen-awesome.css)
    set(DOXYGEN_HTML_EXTRA_FILES
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-fragment-copy-button.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-paragraph-link.js
        ${doxygenawesome_SOURCE_DIR}/doxygen-awesome-darkmode-toggle.js
    )

    # Set stricter warnings for better documentation quality
    set(DOXYGEN_WARN_IF_UNDOCUMENTED YES)
    set(DOXYGEN_WARN_IF_DOC_ERROR YES)
    set(DOXYGEN_WARN_NO_PARAMDOC YES)
    set(DOXYGEN_WARN_AS_ERROR YES) # Treat warnings as errors for CI

    # Exclude certain files or directories from documentation (if needed)
    set(DOXYGEN_EXCLUDE_PATTERNS "${PROJECT_SOURCE_DIR}/tests/*" "${PROJECT_SOURCE_DIR}/examples/*")

    # Add Doxygen documentation target.
    file(GLOB_RECURSE PROJECT_HEADERS "${PROJECT_SOURCE_DIR}/include/**/*.hpp")
    file(GLOB_RECURSE PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/src/**/*.cpp")
    doxygen_add_docs(
        ordered_map_documentation
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/LICENSE.md
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        COMMENT "Generating Doxygen documentation for the library"
    )
endif()
