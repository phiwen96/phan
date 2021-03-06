# Testing library
include (FetchContent)
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.9.1)

  FetchContent_GetProperties (catch) 

if(NOT catch_POPULATED)
	FetchContent_Populate (catch)
	add_subdirectory (${catch_SOURCE_DIR} ${catch_BINARY_DIR}) 
endif()
# Adds Catch2::Catch2

# Tests need to be added as executables first
add_executable(testlib testlib.cpp)

set_target_properties (testlib PROPERTIES CXX_STANDARD 20)

# I'm using C++17 in the test
target_compile_features(testlib PRIVATE cxx_std_20)

# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries (testlib PRIVATE ph::${CMAKE_PROJECT_NAME} Catch2::Catch2)
target_include_directories (testlib PRIVATE ph::${CMAKE_PROJECT_NAME} Catch2::Catch2)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test (NAME testlibtest COMMAND testlib) # Command can be a target



