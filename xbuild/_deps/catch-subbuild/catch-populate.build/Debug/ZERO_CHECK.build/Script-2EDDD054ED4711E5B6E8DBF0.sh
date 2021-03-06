#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild
  make -f /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild
  make -f /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild
  make -f /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild
  make -f /Users/philipwenkel/GitHub/Phan/xbuild/_deps/catch-subbuild/CMakeScripts/ReRunCMake.make
fi

