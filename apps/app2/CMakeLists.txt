# ph_define_self (self)

# add_executable(${self} MACOSX_BUNDLE *.cpp *.hpp)
# target_compile_features(${self} PRIVATE cxx_std_20)
# set(APP_NAME ${self})


# set_target_properties(${self} PROPERTIES CXX_STANDARD 20)

# target_link_libraries(${self} PRIVATE ${CMAKE_PROJECT_NAME})

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
