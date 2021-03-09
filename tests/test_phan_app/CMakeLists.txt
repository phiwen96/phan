ph_define_self (self)

# message (${self})

ph_fetch (catch2)

add_executable(${self} ${CMAKE_CURRENT_LIST_DIR}/src/${self}.cpp)

# ph_precompile (TARGET ${self}
#   PUBLIC 
#   PCH
#     ${ph_catch2_pch} 
#     ${std_pch} 
#     ${boost_pch}
# )



target_compile_definitions (${self} PRIVATE CATCH_CONFIG_MAIN)


ph_subfiles (TARGET ${CMAKE_CURRENT_LIST_DIR}/testFiles RESULT subfiles)

set (i "0")

set (testFiles_definitions)

foreach (file ${subfiles})
  add_compile_definitions (${testFiles_definitions} TEST_FILE_${i}="${file}")
  math (EXPR i "${i}+1")
endforeach ()



add_compile_definitions (${testFiles_definitions} TEST_FILE_COUNT=${i})

target_compile_definitions (${self} PRIVATE ${testFiles_definitions})


set_target_properties (${self} PROPERTIES CXX_STANDARD 20)

# I'm using C++17 in the test
target_compile_features(${self} PRIVATE cxx_std_20)



# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries (${self} 
PRIVATE 
    # ${root_name} 
    Catch2::Catch2 
    # ${Catch2_LIBRARIES}
    # ${Boost_LIBRARIES}
)

target_include_directories (${self} 
PRIVATE 
    # ${root_name} 
    # ${CMAKE_CURRENT_BINARY_DIR}
    Catch2::Catch2
    # ${Catch2_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

ph_precompile (TARGET ${self} 
  PUBLIC 
  PCH 
    # ${ph_catch2_pch} 
    ${std_pch} 
    ${boost_pch}
)

# target_sources (${self} PRIVATE test_files/test.hpp)


# add_test (NAME ${self} 
#   COMMAND 
#     $<TARGET_FILE:${self}> 

#   # WORKING_DIRECTORY 
#   #   ${CMAKE_CURRENT_LIST_DIR}/test_files
# )

# set_tests_properties(${self} PROPERTIES TIMEOUT 10)
