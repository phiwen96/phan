cmake_minimum_required (VERSION 3.19.4)



# include (cmake/phake/phake.cmake)

# ph_dont_build_here ()
# ph_git ()





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


# ph_header_list (headers)
# message(${headers})
# message ("$<CONFIG>")
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

project (phan
	
	VERSION 1.0
	LANGUAGES CXX C
    DESCRIPTION "Very nice project"
)



# add_subdirectory (dependencies/const_str)


add_subdirectory (cmake/phake)
add_subdirectory (dependencies/const_str)

# ph_subdirs (TARGET ${CMAKE_CURRENT_LIST_DIR} RESULT deps)
# foreach (dep ${deps})
# 	add_subdirectory (${dep})
	
# endforeach ()

ph_define_self (self)
ph_define_property (root_name phan_lib)


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
    # set (CMAKE_BUILD_TYPE Debug)
	set_property(GLOBAL PROPERTY USE_FOLDERS YES)
	
	if(CMAKE_CONFIGURATION_TYPES)
      if(NOT "Profile" IN_LIST CMAKE_CONFIGURATION_TYPES)
          list(APPEND CMAKE_CONFIGURATION_TYPES Profile)
		  endif()
		if (NOT "Testing" IN_LIST CMAKE_CONFIGURATION_TYPES)
			list(APPEND CMAKE_CONFIGURATION_TYPES Testing)
		endif ()
  	else()
      set(allowableBuildTypes Debug Release Profile Testing)
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
                   STRINGS "${allowableBuildTypes}")
      if(NOT CMAKE_BUILD_TYPE)
          set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
      elseif(NOT CMAKE_BUILD_TYPE IN_LIST allowableBuildTypes)
          message(FATAL_ERROR "Invalid build type: ${CMAKE_BUILD_TYPE}")
      endif()
  	endif()

      


    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror" CACHE STRING "C++ flags")
    # option(CMAKE_EXPORT_COMPILE_COMMANDS ON)

	# set_property(GLOBAL PROPERTY USE_FOLDERS YES)
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






# cmake --build . --target package_source

# cpack -C Release

# cpack -G productbuild -V


set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries)
include(GNUInstallDirs)
  if(WIN32)
      install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
          DESTINATION ${CMAKE_INSTALL_BINDIR}
  ) 
  else()
      install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ) 
  endif()

  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
   set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
   set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

   if(WIN32)
    set(CPACK_GENERATOR ZIP WIX)
  elseif(APPLE)
    set(CPACK_GENERATOR TGZ productbuild)
  elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CPACK_GENERATOR TGZ RPM)
  else()
    set(CPACK_GENERATOR TGZ)
endif()
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME                Phan)
set(CPACK_PACKAGE_VENDOR              Philip)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "CPack example project")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_VERBATIM_VARIABLES YES)
# set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_LIST_DIR}/Description.txt)
# set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_CURRENT_LIST_DIR}/Welcome.txt)
# set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_LIST_DIR}/LICENSE)
# set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_LIST_DIR}/README.md)

set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_SOURCE_IGNORE_FILES
    /\\.git/
    \\.swp
    \\.orig
    /CMakeLists\\.txt\\.user
    /privateDir/
)
# set(CPACK_PRODUCTBUILD_IDENTITY_NAME "Developer ID Installer")







install(TARGETS app
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
)







include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "My funky project")
set(CPACK_PACKAGE_VENDOR "Me, myself, and I")
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "3")
set(CPACK_PACKAGE_VERSION_PATCH "2")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
if(WIN32 AND NOT UNIX)
  # There is a bug in NSI that does not handle full UNIX paths properly.
  # Make sure there is at least one set of four backlashes.
  set(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
  set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
  set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} My Famous Project")
  set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
  set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
  set(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
  set(CPACK_NSIS_MODIFY_PATH ON)
else()
  set(CPACK_STRIP_FILES "bin/MyExecutable")
  set(CPACK_SOURCE_STRIP_FILES "")
endif()
set(CPACK_PACKAGE_EXECUTABLES "MyExecutable" "My Executable")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
SET ( APP_LOCATION "$<TARGET_FILE:app>" )
SET ( CPACK_BUNDLE_STARTUP_COMMAND "bin/phan")
message(${CMAKE_INSTALL_BINDIR})
include(CPack)
include(CPackIFW)