# Ugly Engine

An early stage OpenGL rendering engine - Linux support only

## Setup

Essential installs for C/C++ & CMake on Debian based distros:

    sudo apt install build-essential cmake

Essential installs for OpenGL, glfw and glm on Debian based distros:

    sudo apt install libgl-dev libglm-dev mesa-utils xorg-dev

After cloning the repo, cd into it and pull in the submodule dependencies:

    git submodule init && git submodule update

Then create a build directory, generate the build system and compile:

    mkdir build && cd $_
    cmake ..
    make

Once compiled, run the Sandbox executable:

    ./sandbox/Sandbox

## Info

third party libaries for all stored in vendor

third party libaries for the engine only: engine/vendor
