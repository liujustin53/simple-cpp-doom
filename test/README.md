# WAD File Structure

A text file to be treated as a WAD ("Where's All the Data?") file that holds all the data for a level. Not an actual WAD file, too hard to actually do that. All lumps and their data described here are for the purposes of describing how to create level files for my implementation, not the actual WAD files used by DOOM.

## Header

Always starts with a header

- String "WAD" at start of file
- Integer specifying number of lumps (pieces of data containing specific information required to construct and execute the map)
- Start line of the directory

Example:

```
WAD
9 3
```

## Directory

List of entries associating names of lumps with their data and where to find them

Entry Structure:

- A string defining the name of the lump
- Start line of the lump's data
- Number of entries in the lump (does not include comments)

Example:

```
MAP 13 1
VERTEXES 14 100
SECTORS 115 20
...
```

## Map Data Lumps

Each entry in a lump must be on a single line. If a lump references or holds another data structure, it is that data structure's index in the lump they belong to (1-indexed). These lumps must follow strictly in this order (maybe?)

- MAP
  - Just a string with a map name
  - Signifies the start of the map data lumps, all map data lumps must follow after this
- VERTEXES
  - A list of each vertex in the map
  - Data
    - x and y coordinates
- SECTORS
  - A list of sectors
  - Data
    - Floor height
    - Ceiling height
    - Texture indices for floor and ceiling texture
      - For our purposes, 3 rgb values between 0-255 will suffice
    - Brightness values for floor, ceiling, thing, wall top, wall bottom exactly in that order
      - -255 to 255, -255 being pure black, 0 being normal color, and 255 being pure white
- SIDEDEFS
  - A list of the sidedefs that are linked to the linedefs
  - Data
    - Texture indices for upper, lower, and middle texture
      - Again, 3 rgb values between 0-255 for upper, lower, and middle color will suffice
    - Sector this sidedef faces
- LINEDEFS
  - A list of all the linedefs in this map
  - Data
    - Start Vertex
    - End Vertex
    - 0 for one-sided (no back sidedef), 1 for two-sided
    - Front and Back sidedefs (if any)
- SEGS
  - A list of line segments called segs that form subsectors
  - Must be listed such that all segs in a subsector are consecutive
  - Data
    - Start Vertex
    - End Vertex
    - Linedef this seg belongs to
    - Side of linedef that it runs on
      - 0 for front side, 1 for back side
    - Offset
      - Distance along linedef to start seg
- SSECTORS
  - A list of subsectors, which are convex polygons where no seg will block the view of any other seg in said subsector
  - Data
    - Index of first seg
    - Seg count
- NODES
  - The node tree which Doom uses to speed up the rendering process
  - Data
    - x and y coordinates of partition line start
    - dx and dy from start to end of partition line
    - Right and left children bounding boxes
      - Top, bottom, left, and right coordinates in that order for each
    - Right and left children nodes
      - A negative number instead describes a subsector number
- THINGS
  - A list of all the Things present in this map
  - Data
    - x y and z coordinates
    - Initial angle facing
    - Thing type
      - 1 for player
