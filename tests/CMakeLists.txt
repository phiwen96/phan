# Testing library
find_package (Boost 1.74 REQUIRED)


# message ("aa")




# Adds Catch2::Catch2

# Tests need to be added as executables first



# add_subdirectory (test_phan_lib)


ph_subdirs (RESULT hej)
# message (${hej})
foreach (subdir ${hej})
  add_subdirectory (${subdir})
endforeach ()






add_executable (testlib test.cpp)
target_link_libraries (testlib PRIVATE ${root_name} Catch2::Catch2 ${Boost_LIBRARIES})
ph_precompile (TARGET testlib PRIVATE PCH ${std_pch} ${catch_pch} ${boost_pch})