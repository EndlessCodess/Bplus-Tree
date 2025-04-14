#!/bin/bash

cd build

make || error_exit "Build failed."

ctest