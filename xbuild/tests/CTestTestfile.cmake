# CMake generated Testfile for 
# Source directory: /Users/philipwenkel/GitHub/Phan/tests
# Build directory: /Users/philipwenkel/GitHub/Phan/xbuild/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[testlibtest]=] "/Users/philipwenkel/GitHub/Phan/xbuild/tests/Debug/testlib")
  set_tests_properties([=[testlibtest]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/philipwenkel/GitHub/Phan/tests/CMakeLists.txt;30;add_test;/Users/philipwenkel/GitHub/Phan/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[testlibtest]=] "/Users/philipwenkel/GitHub/Phan/xbuild/tests/Release/testlib")
  set_tests_properties([=[testlibtest]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/philipwenkel/GitHub/Phan/tests/CMakeLists.txt;30;add_test;/Users/philipwenkel/GitHub/Phan/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test([=[testlibtest]=] "/Users/philipwenkel/GitHub/Phan/xbuild/tests/MinSizeRel/testlib")
  set_tests_properties([=[testlibtest]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/philipwenkel/GitHub/Phan/tests/CMakeLists.txt;30;add_test;/Users/philipwenkel/GitHub/Phan/tests/CMakeLists.txt;0;")
elseif("${CTEST_CONFIGURATION_TYPE}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[testlibtest]=] "/Users/philipwenkel/GitHub/Phan/xbuild/tests/RelWithDebInfo/testlib")
  set_tests_properties([=[testlibtest]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/philipwenkel/GitHub/Phan/tests/CMakeLists.txt;30;add_test;/Users/philipwenkel/GitHub/Phan/tests/CMakeLists.txt;0;")
else()
  add_test([=[testlibtest]=] NOT_AVAILABLE)
endif()
subdirs("../_deps/catch-build")
