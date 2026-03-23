##
##
##
cmake_minimum_required( VERSION 3.13 )

##
## Project Name
##
project( idTech2 )

##
## Uses STD c++ 20
##
#set( CMAKE_CXX_STANDARD 20 )

##
## External Libraries
##
find_package( SDL3 REQUIRED CONFIG )
include_directories( SDL3::Headers )

##
## set the engine source root path
##
include_directories( ${CMAKE_SOURCE_DIR} )

##
## ctf 
## Caputre The Flag game
add_subdirectory( ctf )

##
## game 
## Game logic 
add_subdirectory( game )

##
## quake2 
## Engine main executable
#add_subdirectory( quake2 )

##
## ref_soft 
## Software renderer
#add_subdirectory( ref_soft )

##
## ref_gl 
## GL renderer
add_subdirectory( ref_gl )
