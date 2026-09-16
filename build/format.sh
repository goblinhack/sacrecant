#!/bin/bash

# Prefer gnu over freebsd coreutils
PATH=/opt/local/libexec/gnubin:$PATH
export PATH

for i in *.cpp *.hpp 
do
  case $i in
    *3rd*)
      ;;
    *stb*)
      ;;
    *lzo*)
      ;;
    *ramdisk*)
      ;;
    *)
      if [[ -f $i ]] ; then
        echo Formatting $i...
        clang-format -i $i &
      fi
    ;;
  esac
done

wait
