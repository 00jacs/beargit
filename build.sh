#!/bin/bash

# This script will take of building (compiling) the beargit program

echo "Building 'beargit'..."
cc ./src/main.c -o beargit.out
echo "Successfully built beargit.out. You can run it with ./beargit.out"
