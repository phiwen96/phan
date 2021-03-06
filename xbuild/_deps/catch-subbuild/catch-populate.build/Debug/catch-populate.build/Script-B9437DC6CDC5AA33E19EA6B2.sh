#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -P /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/tmp/catch-populate-gitclone.cmake
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -E touch /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/src/catch-populate-stamp/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME/catch-populate-download
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -P /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/tmp/catch-populate-gitclone.cmake
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -E touch /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/src/catch-populate-stamp/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME/catch-populate-download
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -P /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/tmp/catch-populate-gitclone.cmake
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -E touch /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/src/catch-populate-stamp/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME/catch-populate-download
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -P /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/tmp/catch-populate-gitclone.cmake
  /usr/local/Cellar/cmake/3.19.6/bin/cmake -E touch /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/catch-populate-prefix/src/catch-populate-stamp/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME/catch-populate-download
fi

