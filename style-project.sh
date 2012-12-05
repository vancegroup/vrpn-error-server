#!/bin/sh

PROJECT=$(cd $(dirname $0) && pwd)
astyle -n --recursive "${PROJECT}/src/*.h" --recursive "${PROJECT}/src/*.cpp"

