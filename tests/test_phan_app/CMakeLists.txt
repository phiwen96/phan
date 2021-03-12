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


ph_subfiles (TARGET ${CMAKE_CURRENT_LIST_DIR}/testFiles_pre RESULT subfiles_abs ABSOLUTE)
# ph_subfiles (TARGET ${CMAKE_CURRENT_LIST_DIR}/testFiles RESULT subfiles_rel RELATIVE)

# foreach (a ${subfiles_rel})
#   message (${a})
# endforeach ()


math (EXPR i 0)

set (testFiles_definitions)

foreach (file_abs ${subfiles_abs})
  message (${file_abs})
  get_filename_component (file_rel ${file_abs} NAME)
  message (${file_rel})
  # add_compile_definitions (${testFiles_definitions} TEST_FILE_${i}="${file}")
 
  add_custom_command (
    TARGET ${self}
    DEPENDS ${file_abs}
    PRE_BUILD
    COMMAND $<TARGET_FILE:phan_app> ${file_abs} ${CMAKE_CURRENT_LIST_DIR}/testFiles_post/${file_rel}
  )
  
  add_compile_definitions (${testFiles_definitions} TEST_FILE_PRE_${i}="${CMAKE_CURRENT_LIST_DIR}/testFiles_pre/${file_rel}")
  add_compile_definitions (${testFiles_definitions} TEST_FILE_POST_${i}="${CMAKE_CURRENT_LIST_DIR}/testFiles_post/${file_rel}")
  add_compile_definitions (${testFiles_definitions} TEST_FILE_FACIT_${i}="${CMAKE_CURRENT_LIST_DIR}/testFiles_facit/${file_rel}")
  target_sources (${self} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/testFiles_pre/${file_rel})
  target_sources (${self} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/testFiles_post/${file_rel})
  target_sources (${self} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/testFiles_facit/${file_rel})
  source_group(test_files\\pre FILES ${CMAKE_CURRENT_LIST_DIR}/testFiles_pre/${file_rel})   # Overrides both of the above
  source_group(test_files\\post FILES ${CMAKE_CURRENT_LIST_DIR}/testFiles_post/${file_rel})   # Overrides both of the above
  source_group(test_files\\facit FILES ${CMAKE_CURRENT_LIST_DIR}/testFiles_facit/${file_rel})
  math (EXPR i ${i}+1)
endforeach ()

# set_target_properties (gpu_gen_definitions PROPERTIES FOLDER "phulkan_generator/Generators/gpu_generate_class/Generators")

# set_target_properties(foo PROPERTIES PROJECT_LABEL "Foo Tastic")
# source_group(Sources REGULAR_EXPRESSION "\\.(c(xx|pp)?|hh?)$")
#   source_group(Tests   REGULAR_EXPRESSION "test.*")  # Overrides the above

# source_group(TREE   ${CMAKE_CURRENT_SOURCE_DIR}
#                PREFIX "Magic\\Sources"
#                FILES  ${subfiles_abs}
# )




add_compile_definitions (${testFiles_definitions} TEST_FILE_COUNT=${i})

# target_compile_definitions (${self} PRIVATE TEST_FILE_COUNT=${i})

target_compile_definitions (${self} PRIVATE ${testFiles_definitions})


set_target_properties (${self} PROPERTIES CXX_STANDARD 20)

# I'm using C++17 in the test
target_compile_features(${self} PRIVATE cxx_std_20)



# Should be linked to the main library, as well as the Catch2 testing library
target_link_libraries (${self} 
PRIVATE 
    # ${root_name} 
    Catch2::Catch2 
    const_str
    # ${Catch2_LIBRARIES}
    # ${Boost_LIBRARIES}
)

target_include_directories (${self} 
PRIVATE 
    # ${root_name} 
    # ${CMAKE_CURRENT_BINARY_DIR}
    Catch2::Catch2
    const_str
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
