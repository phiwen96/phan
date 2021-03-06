#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild
  make -f /Users/philipwenkel/GitHub/Phan/xbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild
  make -f /Users/philipwenkel/GitHub/Phan/xbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild
  make -f /Users/philipwenkel/GitHub/Phan/xbuild/CMakeScripts/ReRunCMake.make
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/philipwenkel/GitHub/Phan/xbuild
  make -f /Users/philipwenkel/GitHub/Phan/xbuild/CMakeScripts/ReRunCMake.make
fi

