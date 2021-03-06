
# ph_current_dir_name (self)
# set (root_source_dir )

include(CheckCXXSymbolExists)

check_cxx_symbol_exists(KUKEN ${root_include_dir}/template_library/template_library.hpp resultVar)

if (resultVar)
    message("is cool!")
else()
    message("not cool!")
endif()

message("${resultVar}")

set(CMAKE_REQUIRED_DEFINES -DFOOBXX=1)

file (GLOB_RECURSE HEADER_LIST ${root_include_dir}/*.hpp)
file (GLOB_RECURSE SOURCE_LIST ${root_src_dir}/*.cpp)

# file(GLOB_RECURSE HEADER_LIST "${phulkan_SOURCE_DIR}/include/phulkan/*.hpp")
# file(GLOB SOURCE_LIST ${phulkan_SOURCE_DIR}/src/*.cpp)

# add_subdirectory (generators)

ph_define_self (self)

ph_get_property (root_name root_name)


add_library (${root_name} STATIC ${SOURCE_LIST} ${HEADER_LIST})

target_compile_features (${root_name} PRIVATE cxx_std_20)

get_property(ResultVariable TARGET ${root_name} PROPERTY CXX_STANDARD)

add_library(ph::${root_name} ALIAS ${root_name})

set(ENABLE_ALGO_BETA 0 CACHE BOOL "")

# get_directory_property(subdir_list DIRECTORY ${CMAKE_CURRENT_LIST_DIR} SUBDIRECTORIES)
# foreach(subdir ${subdir_list})
#   message(STATUS "${subdir}")
# endforeach()

# target_sources (${root_name} PRIVATE "${PROJECT_BINARY_DIR}/include/version.hpp")

configure_file (
    "${PROJECT_SOURCE_DIR}/include/version.hpp.in"
    "${PROJECT_BINARY_DIR}/include/version.hpp"
    @ONLY
)


target_link_libraries(${root_name} PUBLIC 
    $<$<BOOL:${ENABLE_ALGO_BETA}>:algo_beta>
    # $<IF:$<CONFIG:Debug>,checkedAlgo,fastAlgo>
)

# generate (phulkan_generator ${CMAKE_CURRENT_SOURCE_DIR}/include/phulkan_generator/instance.hpp ${CMAKE_CURRENT_SOURCE_DIR}/include/phulkan_generator/gpu.hpp)


target_include_directories (${root_name} PUBLIC ${root_include_dir})
target_include_directories (${root_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


target_include_directories(
    ${root_name} 
 PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/${root_name}>
    $<INSTALL_INTERFACE:include/${root_name}>
 PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)










# set_target_properties (${CMAKE_PROJECT_NAME} PROPERTIES
#     OUTPUT_NAME ${CMAKE_PROJECT_NAME}
#     MACOSX_BUNDLE TRUE
#     MACOSX_RPATH TRUE
#     MACOSX_FRAMEWORK_IDENTIFIER com.testcmake.bundlename
#     XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@loader_path/Libraries"
#     RESOURCE "${RESOURCE_FILES}"
#     XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME TRUE
#     XCODE_ATTRIBUTE_EXECUTABLE_NAME "exeName"
# )


# install(TARGETS ${root_name} EXPORT ${root_name}-export
#     LIBRARY DESTINATION lib
#     ARCHIVE DESTINATION lib
# )

# install(EXPORT ${root_name}-export FILE
#     ${root_name}Targets.cmake NAMESPACE
#     ${root_name}:: DESTINATION lib/cmake/${root_name} 
# )

# file(GLOB_RECURSE headers ${root_include_dir}/*.hpp)

# install(FILES ${headers}
# DESTINATION "include/${root_name} ")

# install(FILES ${PROJECT_SOURCE_DIR}/cmake/${root_name}Config.cmake DESTINATION "lib/cmake/${root_name}" )




