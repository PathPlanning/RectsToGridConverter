cmake_minimum_required(VERSION 2.8)
project(Converter)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -Wextra")
set(SOURCE_FILES
        clipper/clipper.hpp
	clipper/clipper.cpp
	tinyxml/tinystr.cpp
	tinyxml/tinystr.h
	tinyxml/tinyxml.cpp
	tinyxml/tinyxml.h
	tinyxml/tinyxmlerror.cpp
	tinyxml/tinyxmlparser.cpp
        bresenham.h
	Converter.pro
	filler.cpp
	filler.h
	gl.const.h
	main.cpp
	map.cpp
	map.h
	parser.h
	point.h)
add_executable(Converter ${SOURCE_FILES})

