#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-src
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -P /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/tmp/catch-populate-gitupdate.cmake
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-src
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -P /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/tmp/catch-populate-gitupdate.cmake
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-src
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -P /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/tmp/catch-populate-gitupdate.cmake
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-src
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -P /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/tmp/catch-populate-gitupdate.cmake
fi

