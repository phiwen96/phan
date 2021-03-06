# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.19.6)

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(catch-populate NONE)

include(ExternalProject)
ExternalProject_Add(catch-populate
                     "UPDATE_DISCONNECTED" "False" "GIT_REPOSITORY" "https://github.com/catchorg/Catch2.git" "GIT_TAG" "v2.9.1"
                    SOURCE_DIR          "/Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-src"
                    BINARY_DIR          "/Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
)


