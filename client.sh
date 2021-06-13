#!/bin/bash
for i in {1..$4}
do
    ./client $1 $2 $3
done
