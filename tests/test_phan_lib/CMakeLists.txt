ph_define_self (self)

add_executable(${self} src/${self}.cpp)

ph_precompile (TARGET ${self} 
  PRIVATE 
  PCH
    ${ph_catch2_pch} 
    ${std_pch} 
    ${boost_pch}
)

target_compile_definitions (${self} PRIVATE CATCH_CONFIG_MAIN)


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
    Catch2::Catch2
    # ${Catch2_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

# If you register a test, then ctest and make test will run it.
# You can also run examples and check the output, as well.
add_test (NAME ${self} COMMAND $<TARGET_FILE:${self}> hej) # Command can be a target

