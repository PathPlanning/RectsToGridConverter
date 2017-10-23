# RectsToGridConverter
Converter of a set of polygons (defined as a sequences of (x,y) coordinates), placed inside a square/rectangular workspace, to square/rectangular gridng.

Description
==========
This project contains implementations of the following converter: 

the converter takes a set of polygons, defined as a sequences of rational coordinates, size of current workspace in rational numbers and optionally start and goal rational coordinates (the discretization will be according to the start position, so that the object will always be in the center of the cell); the polygon definition must include all vertices of the polygon in strict order; polygons can extend beyond borders of current workspace or overlap; the convertor transform a set o polygons to integer square/rectangular grid using predefined parameter - discretization seed.

Agent's environment with obstacles, start and goal vertices before discretization:

![1](https://user-images.githubusercontent.com/17624024/31894564-89c0d860-b817-11e7-8798-f307bb7ef889.jpg)

And after we have:

| ![photo5435924095918254127](https://user-images.githubusercontent.com/17624024/31895026-c9801532-b818-11e7-9219-201bd29ecded.jpg)] | ![photo5435924095918254126](https://user-images.githubusercontent.com/17624024/31895027-cb3c11f0-b818-11e7-8442-3017d3ee65d9.jpg) |
|:---:|:---:|
| Discrete environment with seed value equals 1 | Discrete environment with seed value equals 0.5 |


In the process there is also polygon offseting algorithm, which offset polygons depending on the radius of a moving agent. 

Converter takes tree parameters: 
* XML file with defined set of polygons
* predefined rational discretization seed 
* predefined rational agent radius

Rational discretization seed - rational number, which defines the process of discretization (transferring continuous map represintation into discrete counterpart). The value of the discretization defines the size (in meters) of one cell on discrete grid. Accordingly, the larger discretization seed value, the smaller the grid and the more obscure the obstacles. Default value - 1. 

Rational agent radius - rational number, which defines the radius of an agent (assuming that we can approximize it's shape to a disk). Default value - 0 (agent is a particle).

Project Purpose
===============
The main purpose of the project is to analyse the difference between different discretization values. The convertation from a set of polygons to discrete grid is faster with larger value of discretization seed, while it can harm the map exactitude. Beside convertation, general pathfinding algorithms also work faster on smaller grids. 

The main question is whether there is some point at which we can accelerate computational process safely for accuracy or not?


Build and Launch
================
Built current project using **Qt Creator** or **CMake**.
Your compiler must support C++ 11 standard.
To launch the compiled file and get a result you need parameters - an input XML file, your discretization seed and radius of the agent.

Some examples are given in folder [samples](https://github.com/PathPlanning/RectsToGridConverter/tree/master/samples).

Download project using:

`git clone https://github.com/PathPlanning/RectsToGridConverter.git`

or downolad the archive directly.

Using **QT Creator**: Then open `.pro` file with **Qt Creator**.

Using **CMake**: Run terminal. Build project using:

```bash
cd PATH_TO_PROJECT
cmake .
make
```
And then run the project with the following arguments (path_to_map_file, descretization_seed=1, agent_radius=0):

`./Converter PATH_TO_XML_FILE 1 0`


Input and Output files
======================
Input and Output files are XML files. Both contain map, but in different representations (input - continious map, set of polygons; output - discrete map, square/rectangular grid).

You can find examples of input and output files in folder [samples](https://github.com/PathPlanning/RectsToGridConverter/tree/master/samples).

#### Input file should contain:
* Mandatory tag `<map>`. It describes the environment.
    * `<abs_height>` and `<abs_width>` - mandatory tags that define actual size of the map. Origin is in the upper left corner. (0,0) - is upper left, (*width*, *height*) is lower right.
    * `<abs_startx>` and `<abs_starty>` - optional tags that defines actual coordinates of the start location. If there is no start location given, the map would be converted as if start location was (0, 0). 
    * `<abs_finishx>` and `<abs_finishy>` - optional tags that horizontal (X) and vertical (Y) offset of the actual goal location.
    * `<obstacles>` - mandatory tag that describes the set of polygons. It has an attribute - "quantity" that defines the number of obstacles. It consists of `<obstacle>` tags that describes each obstacle individually. Each `<obstacle>` tag containes a sequence of `<point>` tag that describes vertices. Each `<point>` tag has two attributes: "x" and "y" - rational coordinates of that point.
    * `<title>`, `<URL>`, `<coordinates>`, etc - optional tags containing additional information on the map.

 #### Output file structure: 
* `<map>`
    * `<height>` and `<width>` - tags that defines integer size of the map after discretization. Origin is in the upper left corner.
    * `<startx>`, `<starty>` - optional tags that defines integer coordinates of the start location in case it was given.
    * `<finishx>`, `<finishy>` - optional tags that defines integer coordinates of the finish location in case it was given.
    * `<grid>` - tag that describes the square/rectangular grid map. It consists of `<row>` tags. Each `<row>` contains a sequence of "0" and "1", where "0" - traversable cell, "1" - non-traversable cell.
    * `<cellsize>` - tag that defines the size of one cell. Depends on the initial discretisation seed.

