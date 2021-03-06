# ph_define_self(this dir)
# message("${this}")
# message("${this}")

# ph_define_self (this dir)
add_subdirectory (app1)
add_subdirectory(app2)



# get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} ABSOLUTE DIRECTORY)
# file(GLOB subdirs LIST_DIRECTORIES true ${this})
# foreach(i ${subdirs})
#     message("KISS")
#     message(${i})
# endforeach()

# ph_subdir_list (dirs ${this})
# message(${this})
# message(${this})
# foreach(i ${dirs})
#     message(${i})
#     add_subdirectory (${i})
# endforeach()
