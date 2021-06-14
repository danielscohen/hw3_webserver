#!/bin/bash
for i in $(seq "$4")
do
    echo "sending...\n"
    ./client $1 $2 $3 &
done
