#!/bin/bash

id=0

while [[ 1 ]]; do
    curl 0.0.0.0:${1} -s > /dev/null
    ((id+=1))
    if [[ $(expr $id % 10000) -eq 0 ]]; then
        exit 0
    fi
done
