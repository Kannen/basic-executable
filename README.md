# Freestanding c++

Library used to compile c++ code in a freestanding environment.

It imports libstdc++ headers that are usable in such environment and provides basic facilities

It also provides some of the functions that gcc expects to be always defined: `strlen`, `strcmp`, `memcpy`, etc.
