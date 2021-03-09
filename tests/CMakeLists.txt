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

ph_fetch (catch2)
# Adds Catch2::Catch2

# Tests need to be added as executables first


add_subdirectory (test_phan_app)

add_subdirectory (test_phan_lib)




