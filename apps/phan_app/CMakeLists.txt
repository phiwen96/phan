ph_define_self (self)

 # Define two executables which each have their own source
  # file as well as the object files from objLib
#   add_executable(app1 app1.cpp $<TARGET_OBJECTS:objLib>)
#   add_executable(app2 app2.cpp $<TARGET_OBJECTS:objLib>)

# target_link_libraries(myApp PRIVATE
# $<IF:$<CONFIG:Debug>,checkedAlgo,fastAlgo>
# )




function (test)


    function (kuk)
        
    endfunction (kuk)
    
    function (FITTA a) 
        # message ("${a}")
    endfunction ()

    function (SNOR...)
        message ("${ARGN}")
        # message ("${b}")
    endfunction ()

    function (f0_N)
        # message ("${ARGN}")
    endfunction ()


    ph_parse (n0 KUK0 kuk n1 HORA FITTA n SNOR f0 hej0 f1 hej1 fn hejn ARGS ${ARGN})

    

endfunction ()

# test (kuk f0 "snoppen" horan kuk SNOR d aaa FITTA ss)




find_package (Boost 1.75 REQUIRED)

# target_precompile_headers(<target> REUSE_FROM <other_target>)

add_executable(${self} main.cpp)

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
set(APP_NAME ${self})


set_target_properties(${self} PROPERTIES CXX_STANDARD 20)

target_link_libraries(${self} PRIVATE ${root_name} ${Boost_LIBRARIES})
target_include_directories (${self} PRIVATE ${root_name} ${Boost_INCLUDE_DIRS})

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