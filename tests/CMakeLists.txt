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




