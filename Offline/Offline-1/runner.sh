#!/bin/bash
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <program_number>"
    exit 1
fi

program_number=$1

if [ "$program_number" -eq 1 ]; then
    g++ -g 1905067_camera.cpp -o demo -lglut -lGLU -lGL
    ./demo
elif [ "$program_number" -eq 2 ]; then
    g++ -g 1905067_rolling_ball_final.cpp -o demo -lglut -lGLU -lGL
    ./demo
elif [ "$program_number" -eq 3 ]; then
    g++ -g 1905067_magicCube.cpp -o demo -lglut -lGLU -lGL
    ./demo
else
    echo "Invalid program number. Choose 1, 2, or 3."
    exit 1
fi
