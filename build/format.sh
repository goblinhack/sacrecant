#!/bin/bash

# Prefer gnu over freebsd coreutils
PATH=/opt/local/libexec/gnubin:$PATH
export PATH

for i in *.cpp *.hpp 
do
  case $i in
    *stb*)
      ;;
    *lzo*)
      ;;
    *ramdisk*)
      ;;
    *)
      echo Formatting $i...
      clang-format -i $i &
    ;;
  esac
done

wait
