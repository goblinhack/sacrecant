#!/bin/bash
#
# Used to sort declarations in the code
#
# Use "do" as the first arg if the output looks sane.
#

# Prefer gnu over freebsd coreutils
PATH=/opt/local/libexec/gnubin:$PATH
export PATH

process_file() {
    PRE=$(mktemp) || exit 1
    PAYLOAD=$(mktemp) || exit 1
    POST=$(mktemp) || exit 1
    OUT=$(mktemp) || exit 1

    FILE=$1

    for WHICH in 1 2 3 4 5 6 7 8 9
    do
      sed "1,/begin sort marker${WHICH}/!d" $IN > $PRE
      sed "/begin sort marker${WHICH}/,/end sort marker${WHICH}/!d" $IN | tail -n +1 - | grep -v "sort marker" | sort > $PAYLOAD
      sed "/end sort marker${WHICH}/,\$!d" $IN | tail -n +1 - > $POST
      sort $PAYLOAD | uniq > $PAYLOAD.tmp
      mv $PAYLOAD.tmp $PAYLOAD
      cat $PRE $PAYLOAD $POST > $OUT
      diff $OUT $IN
      if [ "$1" = "do" ]; then
          mv $OUT $IN
      fi
    done
}

for IN in $(grep "begin sort marker" --files-with-matches *.hpp *.cpp 2>/dev/null)
do
    if [ ! -f $IN ];
    then
      continue
    fi

    echo Processing sort markers $IN...
    process_file $IN &
done

wait
