# ph_define_self (self)
set (self app)

 # Define two executables which each have their own source
  # file as well as the object files from objLib
#   add_executable(app1 app1.cpp $<TARGET_OBJECTS:objLib>)
#   add_executable(app2 app2.cpp $<TARGET_OBJECTS:objLib>)

# target_link_libraries(myApp PRIVATE
# $<IF:$<CONFIG:Debug>,checkedAlgo,fastAlgo>
# )




function (test)


    function (kuk)
        message ("KKKKUUUUUUKKK")
    endfunction (kuk)
    
    function (FITTA a) 
        message ("${a}")
    endfunction ()

    function (SNOR...)
        message ("${ARGN}")
        # message ("${b}")
    endfunction ()

    function (f0_N)
        # message ("${ARGN}")
    endfunction ()


    # ph_parse (. kuk .. HORA FITTA ... SNOR f0 hej0 f1 hej1 fn hejn args ${ARGN})
    ph_parse (. kuk .. kuks ... kukar args ${ARGN})
    

endfunction ()

# test (kuk f0 "snoppen" horan kuk SNOR d aaa FITTA ss)
# test (kuk)


find_package (Boost 1.74 REQUIRED)

# target_precompile_headers(<target> REUSE_FROM <other_target>)

file (GLOB_RECURSE HEADER_LIST ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp)
file (GLOB_RECURSE SOURCE_LIST ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)

source_group("sources" FILES SOURCE_LIST)

add_executable(${self} ${SOURCE_LIST} ${HEADER_LIST})

# set_target_properties(${self} PROPERTIES OUTPUT_NAME "files")

set_target_properties (${self} PROPERTIES FOLDER "phan")

target_compile_definitions (${self} PRIVATE $<$<CONFIG:Debug>:Debug> $<$<CONFIG:Release>:Release> $<$<CONFIG:Kuk>:Kuk>)

set_target_properties(${self} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

target_include_directories (${self} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

ph_precompile (TARGET ${self} PRIVATE PCH ${std_pch} ${boost_pch})



# add_executable(${self} MACOSX_BUNDLE main.cpp)

# Only suitable if SOURCES does not contain generated files in this example
# get_target_property(sources ${self} SOURCES)
# source_group(TREE   ${CMAKE_CURRENT_SOURCE_DIR}
#              PREFIX "Sourcess"
#              FILES  main.cpp
# )

# configure_file (app.hpp.in app.hpp @ONLY)
# configure_file (app_2_.hpp app_2.hpp @ONLY)




target_include_directories (${self} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}) 

# target_sources (${self} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/app.hpp)
# target_sources (${self} PRIVATE app.hpp.in)
# target_sources (${self} PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/app_2.hpp)
# target_sources (${self} PRIVATE app_2_.hpp)


target_compile_features(${self} PRIVATE cxx_std_20)



set_target_properties(${self} PROPERTIES CXX_STANDARD 20)

target_link_libraries(${self} PRIVATE ${root_name} const_str ${Boost_LIBRARIES})
target_include_directories (${self} PRIVATE ${root_name} const_str ${Boost_INCLUDE_DIRS})
ph_precompile (TARGET ${self} PRIVATE ${boost_pch} ${std_pch})

# add_custom_target(Docs SOURCES ${PROJECT_SOURCE_DIR}/README.md ${PROJECT_SOURCE_DIR}/.gitignore ${PROJECT_SOURCE_DIR}/LICENSE)

# # Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}
# install(TARGETS ${self}
#     BUNDLE DESTINATION . COMPONENT Runtime
#     RUNTIME DESTINATION bin COMPONENT Runtime
# )

# # Note Mac specific extension .app
# set(APPS "\${CMAKE_INSTALL_PREFIX}/${APP_NAME}.app")

# # Directories to look for dependencies
# set(DIRS ${CMAKE_BINARY_DIR})

# install(CODE "include(BundleUtilities)
#     fixup_bundle(\"${APPS}\" \"\" \"${DIRS}\")")

# set(CPACK_GENERATOR "DRAGNDROP")
# include(CPack)



# #include <boost/preprocessor/seq/for_each.hpp>
# #include <boost/preprocessor/variadic/to_seq.hpp>
# #include <boost/preprocessor/facilities/expand.hpp>
# #include <boost/preprocessor/stringize.hpp>
# #include <boost/preprocessor/repetition/enum_params.hpp>
# #include <boost/preprocessor/cat.hpp>
# #include <boost/preprocessor/seq/cat.hpp>
# #include <boost/preprocessor/control/if.hpp>
# #include <boost/preprocessor/facilities/empty.hpp>
# #include <boost/preprocessor/facilities/identity.hpp>
# #include <boost/preprocessor/seq/to_tuple.hpp>
# #include <boost/preprocessor/seq/for_each_i.hpp>
# #include <boost/preprocessor/arithmetic/inc.hpp>
# #include <boost/preprocessor/comparison/not_equal.hpp>
# #include <boost/preprocessor/repetition/for.hpp>
# #include <boost/preprocessor/tuple/elem.hpp>
# #include <boost/preprocessor/repetition.hpp>
# #include <boost/preprocessor/arithmetic/sub.hpp>
# #include <boost/preprocessor/punctuation/comma_if.hpp>
# #include <boost/preprocessor/control/if.hpp>
# #include <boost/preprocessor/facilities/empty.hpp>
# #include <boost/preprocessor/facilities/identity.hpp>
# #include <const_str/const_str.hpp>



# function(generate target instance_file gpu_file)
# 	add_custom_command (
# 		TARGET ${target}
# 		PRE_BUILD
# 		COMMAND $<TARGET_FILE:gpu_gen_definitions> ${instance_file} ${gpu_file})
# endfunction()







set_target_properties(${self} PROPERTIES
      MACOSX_BUNDLE_BUNDLE_VERSION       "${PROJECT_VERSION}${BUILD_SUFFIX}"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
  )

#   set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM   "ABC12345DE"    CACHE STRING "")
  set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Philip Wenkel" CACHE STRING "")

  # Apps must have install step enabled
  set_target_properties(${self} PROPERTIES
      XCODE_ATTRIBUTE_SKIP_INSTALL  NO
      XCODE_ATTRIBUTE_INSTALL_PATH  "$(LOCAL_APPS_DIR)"
)


# if(APPLE)
#       set(basePoint @loader_path)
#   else()
#       set(basePoint $ORIGIN)
#   endif()
#   include(GNUInstallDirs)
#   file(RELATIVE_PATH relDir
#        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}
#        ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}
#   )
#   set(CMAKE_INSTALL_RPATH ${basePoint} ${basePoint}/${relDir})

include(InstallRequiredSystemLibraries)
include(GNUInstallDirs)

install(TARGETS ${self} 
CONFIGURATIONS Release
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS ${self} 
CONFIGURATIONS Debug
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

