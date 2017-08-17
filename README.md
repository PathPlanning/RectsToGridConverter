# RectsToGridConverter
Converter of a set of polygons (defined as a sequences of (x,y) coordinates), placed inside a square/rectangular workspace, to square/rectangular gridng.

Description
==========
This project contains implementations of the following converter: 

the converter takes a set of polygons, defined as a sequences of rational coordinates, size of current workspace in rational numbers and optionally start and goal rational coordinates (the discretization will be according to the start position, so that the object will always be in the center of the cell); the polygon definition must include all vertices of the polygon in strict order; polygons can extend beyond borders of current workspace or overlap; the convertor transform a set o polygons to integer square/rectangular grid using predefined parameter - discretization seed.

Converter takes two parameters: 
* XML file with defined set of polygons
* predefined rational discretization seed 

Rational discretization seed - rational number, which defines the process of discretization (transferring continuous map represintation into discrete counterpart). The value of the discretization defines the size (in meters) of one cell on discrete grid. Accordingly, the larger discretization seed value, the smaller the grid and the more obscure the obstacles.


Project Purpose
===============
The main purpose of the project is to analyse the difference between different discretization values. The convertation from a set of polygons to discrete grid is faster with larger value of discretization seed, while it can harm the map exactitude. Beside convertation, general pathfinding algorithms also work faster on smaller grids. 

The main question is whether there is some point at which we can accelerate computational process safely for accuracy or not?


Build and Launch
================
To build the project you can use QtCreator.
Your compiler must support C++ 11 standard.
To launch the compiled file and get a result you need parameters - an input XML file and your discretization seed. Some examples are given in folder [samples](https://github.com/PathPlanning/RectsToGridConverter/tree/master/samples).

Input and Output files
======================
Input and Output files are XML files. Both contain map, but in different representations (input - continious map, set of polygons; output - discrete map, square/rectangular grid).

You can find examples of input and output files in folder [samples](https://github.com/PathPlanning/RectsToGridConverter/tree/master/samples).

#### Input file should contain:
  * **\<map>** - mandatory tag. It describes the environment.
  * **\<abs_height>** and **\<abs_width>** - mandatory tags that defines actual size of the map. Origin is in the upper left corner.
  * **\<abs_startx>**, **\<abs_starty>** - optional tags that defines actual coordinates of the start location. If there is no start location given, the map would be converted as if start location was (0, 0). 
  * **\<abs_finishx>**, **\<abs_finishy>** - optional tags that defines actual coordinates of the finish location.
  * **\<obstacles>** - mandatory tag that describes the set of polygons. It has an attribute - "quantity" that defines the number of obstacles. It consists of **\<obstacle>** tags that describes each obstacle individually. Each **\<obstacle>** tag containes a sequence of **\<point>** tag that describes vertices. Each **\<point>** tag has two attributes: "x" and "y" - rational coordinates of that point.

 #### Output file structure: 
  * **\<map>**
  * **\<height>** and **\<width>** - tags that defines integer size of the map after discretization. Origin is in the upper left corner.
  * **\<startx>**, **\<starty>** - optional tags that defines integer coordinates of the start location in case it was given.
  * **\<finishx>**, **\<finishy>** - optional tags that defines integer coordinates of the finish location in case it was given.
  * **\<grid>** - tag that describes the square/rectangular grid map. It consists of **\<row>** tags. Each **\<row>** contains a sequence of "0" and "1", where "0" - traversable cell, "1" - non-traversable cell.
  * **\<cellsize>** - tag that defines the size of one cell. Depends on the initial discretisation seed.

