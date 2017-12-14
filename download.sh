#!/bin/sh
MOUNTAIN_OBJ="https://filedn.com/lXE33vJSKAGY4tsHPBP3yBb/mountains.obj"

file="./models/mountains/mountains.obj"
if [ ! -f "$file" ]; then
    curl $MOUNTAIN_OBJ --output $file
fi