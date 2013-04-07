cmake_minimum_required(VERSION 2.8.1)

project( lruapp )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++0x )  

file( GLOB_RECURSE SOURCES 
      RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
      ${CMAKE_CURRENT_SOURCE_DIR}/lru/*.h 
      ${CMAKE_CURRENT_SOURCE_DIR}/lru/*.hpp 
      ${CMAKE_CURRENT_SOURCE_DIR}/lru/*.hxx 
      ${CMAKE_CURRENT_SOURCE_DIR}/lru/*.cpp )

add_executable( ${PROJECT_NAME} ${SOURCES} ${HEADERS} )
