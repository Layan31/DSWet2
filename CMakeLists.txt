cmake_minimum_required(VERSION 3.28)
project(DSWet2)

set(CMAKE_CXX_STANDARD 17)

add_executable(DSWet2
        HashNode.h
        DynamicHashTable.h
        UFNode.h
        UnionFind.h
        dspotify25b2.h
        main25b2.cpp
        dspotify25b2.cpp
        wet2util.h)
