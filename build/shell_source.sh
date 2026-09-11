#!/bin/bash
#
# Used to sort declarations in the code
#
# Use "do" as the first arg if the output looks sane.
#

# Prefer gnu over freebsd coreutils
PATH=/opt/local/libexec/gnubin:$PATH
export PATH

PRE=$(mktemp) || exit 1
PAYLOAD=$(mktemp) || exit 1
SHELL=$(mktemp) || exit 1
POST=$(mktemp) || exit 1
OUT=$(mktemp) || exit 1

for IN in $(grep "begin shell marker" --files-with-matches *.hpp *.cpp 2>/dev/null)
do
    if [ ! -f $IN ];
    then
      continue
    fi

    echo Processing shell $IN...
    for WHICH in 1 2 3 4 5 6 7 8 9
    do
      grep -q "shell marker${WHICH}" $IN
      if [ $? != 0 ]; then
        continue
      fi

      sed "1,/begin shell marker${WHICH}/!d" $IN | head -n -1 - > $PRE
      sed "/begin shell marker${WHICH}/,/end shell marker${WHICH}/!d" $IN | grep "\* shell" | sed -e 's/\/\* shell //g' -e 's/\*\/$//g' > $PAYLOAD
      sed "/begin shell marker${WHICH}/,/end shell marker${WHICH}/!d" $IN | grep "shell" | grep -v "end shell marker${WHICH}" > $SHELL
      sed "/end shell marker${WHICH}/,\$!d" $IN | tail -n +2 - > $POST
      cat $PRE > $OUT
      cat $SHELL >> $OUT
      sh $PAYLOAD >> $OUT
      echo "  /* end shell marker${WHICH} */" >> $OUT
      cat $POST >> $OUT
      diff $OUT $IN
      if [ "$1" = "do" ]; then
          mv $OUT $IN
      fi
    done
done
