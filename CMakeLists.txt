cmake_minimum_required (VERSION 3.19.4)
include (cmake/functions.cmake)
include (cmake/libs.cmake)
ph_dont_build_here ()
ph_git ()





get_filename_component(root_src_dir src ABSOLUTE CACHE "")
get_filename_component(root_include_dir include ABSOLUTE CACHE "")
get_filename_component(root_binary_dir ${CMAKE_CURRENT_BINARY_DIR} ABSOLUTE CACHE "")
get_filename_component(root_dir ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE CACHE "")




# bajs (z1 snipp o1 kuken o2 "hej" n1 sd jn "km" n2 "l," km)


# func (SOURCES foo.cpp bar.cpp TARGET kiss SNOPP myApp ENABLE_NET)
# func (COOL_STUFF TARGET dummy IMAGES here.png there.png gone.png)

# return ()
# variable_watch(myVar [command])
# set_property(GLOBAL PROPERTY source_list_property "bajs")
ph_define_self (self)
ph_define_property (root_name ${self})

set(PACKAGE_VERSION 1)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${self}ConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY AnyNewerVersion
    )

# ph_header_list (headers)
# message(${headers})
message ("$<CONFIG>")
# $<CXX_COMPILER_ID>

# $<TARGET_NAME_IF_EXISTS:tgt>

# $<TARGET_FILE_BASE_NAME:tgt>

# $<TARGET_FILE:tgt>
# Full path to the tgt binary file.

# Base name of tgt, i.e. $<TARGET_FILE_NAME:tgt> without prefix and suffix. For example, if the tgt filename is libbase.so, the base name is base.
# The target name tgt if the target exists, an empty string otherwise.

# $<TARGET_FILE_PREFIX:tgt>
# Prefix of the tgt filename (such as lib).

# $<TARGET_FILE_SUFFIX:tgt>
# Suffix of the tgt filename (extension such as .so or .exe).

# $<SHELL_PATH:...>
# Content of ... converted to shell path style. For example, slashes are converted to backslashes in Windows shells
# return()

# return()

project (
	${self}
	VERSION 1.0
	LANGUAGES CXX C
    DESCRIPTION "Very nice project")


# Packaging support
# set(CPACK_PACKAGE_VENDOR "Vendor name")
# set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Some summary")
# set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
# set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
# set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENCE")
# set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

# set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
# set(CPACK_SOURCE_IGNORE_FILES
#     /.git
#     /dist
#     /.*build.*
#     /\\\\.DS_Store
# )

# include(CPack)


if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set (CMAKE_CXX_STANDARD 20)
	set (CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS        ON)
    set (CMAKE_BUILD_TYPE Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror" CACHE STRING "C++ flags")
    option(CMAKE_EXPORT_COMPILE_COMMANDS ON)

	set_property(GLOBAL PROPERTY USE_FOLDERS YES)
    # set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
    #     STRINGS Debug Release Profile)
    

	# find_package (Boost 1.75 REQUIRED)
	# find_package(Vulkan REQUIRED)
	# find_package(glfw3 REQUIRED)
	# find_package(glm REQUIRED)

	include (CTest)
	
endif()


add_subdirectory (src)
add_subdirectory (apps)

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
  add_subdirectory(tests)
endif()















# # Set Package Name
# set(CPACK_PACKAGE_NAME "example")
# # Enable Customized Installation Directory With ON
# set(CPACK_SET_DESTDIR ON)
# # Set Installation Directory
# set(CPACK_INSTALL_PREFIX "/home/vesoft/install")
# # Set The Version Information of The Installation Package
# set(CPACK_PACKAGE_VERSION "1.0.0")
# # Set The Group Name
# set(CPACK_RPM_PACKAGE_GROUP "vesoft")
# # Set The Vendor Name
# set(CPACK_PACKAGE_VENDOR "vesoft")
# # Set The License Information
# set(CPACK_RPM_PACKAGE_LICENSE "Apache 2.0 + Common Clause 1.0")
# include(CPack)