# ph_define_self (self)
set (self with_files)

# message (${self})

ph_fetch (catch2)

add_executable(${self} ${CMAKE_CURRENT_LIST_DIR}/src/${self}.cpp)



set_target_properties (${self} PROPERTIES FOLDER "phan/app/test")

set_target_properties(${self} PROPERTIES PROJECT_LABEL "files")


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

target_compile_definitions (app 
    PRIVATE 
      $<$<CONFIG:Debug>:TEST_FILES_PRE_PATH="${CMAKE_CURRENT_LIST_DIR}/testFiles_pre/">
      $<$<CONFIG:Debug>:TEST_FILES_POST_PATH="${CMAKE_CURRENT_LIST_DIR}/testFiles_post/">
      $<$<CONFIG:Debug>:TEST_FILES_FACIT_PATH="${CMAKE_CURRENT_LIST_DIR}/testFiles_facit/">
      $<$<CONFIG:Debug>:TEST_FOLDERS_PRE_PATH="${CMAKE_CURRENT_LIST_DIR}/testFolders_pre/">
      $<$<CONFIG:Debug>:TEST_FOLDERS_POST_PATH="${CMAKE_CURRENT_LIST_DIR}/testFolders_post/">
)

# ph_subdirs (TARGET ${CMAKE_CURRENT_LIST_DIR}/testFiles_pre)


target_sources (app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/testFolders_pre ${CMAKE_CURRENT_LIST_DIR}/testFolders_post)

math (EXPR i 0)

set (testFiles_definitions)

# add_custom_target(kiss ALL
#       COMMAND "${CMAKE_COMMAND}" -E remove_directory philip
#       WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/testFolders_post
#       POST_BUILD
# )

foreach (file_abs ${subfiles_abs})
  # message (${file_abs})
  get_filename_component (file_rel ${file_abs} NAME)

  # message (${file_rel})
  # add_compile_definitions (${testFiles_definitions} TEST_FILE_${i}="${file}")
 
  add_custom_command (
    TARGET ${self}
    DEPENDS ${file_abs}
    PRE_BUILD
    COMMAND $<TARGET_FILE:app> ${file_abs} ${CMAKE_CURRENT_LIST_DIR}/testFiles_post/${file_rel}
  )
  target_compile_definitions (app 
    PRIVATE 
      $<$<CONFIG:Debug>:TEST_FILE_COUNT=${i}>
      $<$<CONFIG:Debug>:TEST_FILE_PRE_${i}="${CMAKE_CURRENT_LIST_DIR}/testFiles_pre/${file_rel}">
      $<$<CONFIG:Debug>:TEST_FILE_POST_${i}="${CMAKE_CURRENT_LIST_DIR}/testFiles_post/${file_rel}">
      $<$<CONFIG:Debug>:TEST_FILE_FACIT_${i}="${CMAKE_CURRENT_LIST_DIR}/testFiles_facit/${file_rel}">
  )
  
  # target_sources (${PROJECT_NAME}_app  PRIVATE $<$<CONFIG:Debug>${CMAKE_CURRENT_LIST_DIR}/testFiles_pre/${file_rel}>)
  # target_sources (${PROJECT_NAME}_app  PRIVATE $<$<CONFIG:Debug>${CMAKE_CURRENT_LIST_DIR}/testFiles_post/${file_rel}>)
  # target_sources (${PROJECT_NAME}_app  PRIVATE $<$<CONFIG:Debug>${CMAKE_CURRENT_LIST_DIR}/testFiles_facit/${file_rel}>)
  # source_group(test_files\\pre FILES $<$<CONFIG:Debug>${CMAKE_CURRENT_LIST_DIR}/testFiles_pre/${file_rel}>)   # Overrides both of the above
  # source_group(test_files\\post FILES $<$<CONFIG:Debug>${CMAKE_CURRENT_LIST_DIR}/testFiles_post/${file_rel}>)   # Overrides both of the above
  # source_group(test_files\\facit FILES $<$<CONFIG:Debug>${CMAKE_CURRENT_LIST_DIR}/testFiles_facit/${file_rel}>)


  
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



target_compile_definitions (app PRIVATE $<$<CONFIG:Debug>:TEST_FILE_COUNT=${i}>)


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
