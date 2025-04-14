# Helpers
Collection of utility functions and helper classes for common C++ programming needs all in one code file and header designed to make life easier.
The master branch is targeted for Linux desktops but is compatable with Windows and embedded toolchains with minimal adaptation time.
C++11 is the minimal standard required.

# Objectives
+ Quick and easy inclusion in larger projects.
+ Simple namespace organization to separate helper utilities.
+ No special external dependencies.
+ Platform independence.

# Unit Test
A simple unit test is provided that outlines basic use cases for the namespaces. Refer to them for example code.

# Notes
+ Look at the defines in Helpers.h to see the CRC16/32 parameters. The CRC16 params are configured for CRC-16/MODBUS.

# Build
On linux:
g++ -std=c++11 -o HelpersTest Helpers.cpp Test.cpp
