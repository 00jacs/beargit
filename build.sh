#!/bin/bash

# This script will take of building (compiling) the beargit program

echo "Building 'beargit'..."
cc -o beargit.out ./src/main.c ./src/init.c ./src/commit.c ./src/utils.c ./src/shared.c
echo "Successfully built beargit.out. You can run it with ./beargit.out"
