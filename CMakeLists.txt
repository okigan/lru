cmake_minimum_required(VERSION 2.8.1)

project( lru )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x )  

file( GLOB_RECURSE HEADERS *.h *.hpp *.hxx)
file( GLOB_RECURSE SOURCES *.cpp )

add_library( ${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS} )
