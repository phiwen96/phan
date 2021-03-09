# Testing library
find_package (Boost 1.74 REQUIRED)

ph_fetch (catch2)
# message ("aa")




# Adds Catch2::Catch2

# Tests need to be added as executables first


add_subdirectory (test_phan_app)

# add_subdirectory (test_phan_lib)


ph_subdirs (RESULT hej TARGET ${PROJECT_SOURCE_DIR})
# message (${hej})
foreach (a ${hej})
  message(${a})
endforeach ()




