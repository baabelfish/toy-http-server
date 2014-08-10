#!/bin/bash
while true; do
    killall main
    clear
    make -j $((`cat /proc/cpuinfo|grep processor|wc -l`*2)) && clear && ./main&

    if [[ $(which inotifywait) == "" ]]; then
        sleep 5
    else
        change=$(inotifywait -r -e close_write,moved_to,create {tests,src,CMakeLists.txt} 2> /dev/null)
    fi
done
