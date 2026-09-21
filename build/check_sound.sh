#!/bin/bash

for sound in $( find . -name "*.cpp" | xargs grep "sound_play(g, \"" | sed -e 's/.*sound_play(g, //g' -e 's/).*;//g' | sort | uniq )
do
    find . -name "*.cpp" | xargs grep -q "sound_load.*$sound"
    if [ $? -eq 1 ]; then
        echo ======== ERROR: Sound "$sound": not found =========
        find . -name "*.cpp" | xargs grep "sound_play" | grep $sound
        exit 1
    fi
done

exit 0
