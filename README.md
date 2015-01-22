RandomTeam
==========

Description
-----------

This C++ program allows to play a tournament at the [Multi-Agent Programming
Contest](https://multiagentcontest.org/).

This contest consisted of applying existing or implementing a new multi-agent
system to solve a cooperative task in a highly dynamic environment. In this
environment, represented as a graph, the goal of an agent team is to find and
occupy the best zones.

Build
-----

To download this repository use this command line:

    git clone --recursive https://github.com/tdcdev/RandomTeam

In order to compile you need g++ 4.8 or greater, cmake 2.8.9 or greater and
Boost 1.55. Use the following commands in order to compile:

    mkdir build
    cd build
    cmake ..
    make
    make install

If Boost is not install in a standard location you can use cmake like this:

    BOOST_ROOT=<path-to-boost> cmake ..

If you want install the program in local path you can use cmake like this:

    cmake -DCMAKE_INSTALL_PREFIX=<path-to-install> ..

Usage
-----

Use the following command line:

    RandomTeam -t <team-config-file>

You can found team configuration files [here](res).

Please use this command line for more informations:

    RandomTeam -h

Licence
-------

This software is licensed under the [MIT license](LICENSE.txt).
