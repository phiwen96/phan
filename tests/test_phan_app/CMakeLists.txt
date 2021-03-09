ph_define_self (self)

message (${self})

add_executable(test_phan_app ${CMAKE_CURRENT_LIST_DIR}/src/test_phan_app.cpp)

ph_precompile (TARGET test_phan_app
  PRIVATE 
  PCH
    ${ph_catch2_pch} 
    ${std_pch} 
    ${boost_pch}
)

ph_precompile (TARGET ${self} PRIVATE PCH ${std_pch} ${boost_pch})

target_compile_definitions (test_phan_app PRIVATE CATCH_CONFIG_MAIN)


set_target_properties (${self} PROPERTIES CXX_STANDARD 20)

# I'm using C++17 in the test
target_compile_features(${self} PRIVATE cxx_std_20)


# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries (${self} 
  PRIVATE 
    ${root_name} 
    Catch2::Catch2 
    # ${Catch2_LIBRARIES}
    ${Boost_LIBRARIES}
)

target_include_directories (${self} 
  PRIVATE 
    ${root_name} 
    ${CMAKE_CURRENT_BINARY_DIR}
    Catch2::Catch2
    # ${Catch2_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

target_sources (${self} PRIVATE test_files/test.hpp)


add_test (NAME ${self} 
  COMMAND 
    $<TARGET_FILE:${self}> 

  WORKING_DIRECTORY 
    ${CMAKE_CURRENT_LIST_DIR}/test_files
)

set_tests_properties(${self} PROPERTIES TIMEOUT 10)
