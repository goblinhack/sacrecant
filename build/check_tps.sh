#!/bin/bash

for tp in $( find . -name "*.cpp" | xargs grep "tp_find_mand(\"" | sed -e 's/.*tp_find_mand..//g' -e 's/\".*//g' | sort | uniq )
do
    find . -name "*.cpp" | xargs grep -q "tp_load(\"$tp\")"
    if [ $? -eq 1 ]; then
        echo ======== ERROR: Template "$tp": not found =========
        find . -name "*.cpp" | xargs grep "tp_find_mand(\"" | grep "$tp"
        exit 1
    fi
done

exit 0
