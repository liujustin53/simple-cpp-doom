# DOOM in C++

Attempting to make Doom from scratch in C++ using SFML

# Sources:

- Youtube Playlist of all videos used
  - https://youtube.com/playlist?list=PLfQzhHmKzSPp6EmFB_-2VXIJPu2wy6Mhm
- Wikipedia page for general guideline
  - https://en.wikipedia.org/wiki/Doom_engine#Drawing_the_walls
- Doom Wiki page
  - https://doomwiki.org/wiki/Doom_engine
- GitHub repo of port of Doom from C to C++
  - https://github.com/patricia-gallardo/cpp-doom

# Basic Game Overview

## Map

A level in Doom is actually a 2D map, not a 3D space.

## Game Level/Object Structure

### Vertex

A simple point with x and y coordinates. Also contains the coordinates projected onto the linedef the vertex belongs to.

### Linedef

Lines that make up the shape of the map. Contains one or two sidedefs that have textures. Divides the map into sectors and triggers action specials.

Data Structure:

- Start Vertex
- End Vertex
- dx and dy
- Flag
  - Denotes various properties
- Special Type
  - Creates special effects
- Sector Tag
  - The sector that this linedef acts on
- Array of front sidedef index and back sidedef index
  - -1 if no back

### Sidedef

Texture drawn onto sidedef. References a sector

Data Structure:

- x and y offsets
  - Offsets texture drawn
- Texture indices for top, bottom, and middle texture
- Pointer to sector it faces

### Sector

Area referenced by sidedefs

Data Structure:

- Floor and ceiling height
- Floor texture
- Ceiling texture
- Light level
- Special type
  - Specifies certain effects
- Tag
  - Used to determine if sector is target of linedef

### Blockmap

### Things

Represent objects in the game

Data Structure:
- x y z positions
- Angle facing
- Flags
- ID

### Node

Nodes of a BSP tree that sorts all the subsectors

Data Structure:

- x and y coordinate of partition line start
- dx and dy of partition line from start to end
- Bounding Box of children
- Left and Right children

### Seg

Segments of linedegs, describes portion of linedef that borders the subsector that this seg belongs to.

Data Structure:

- Start vertex
- End vertex
- Offset
  - Distance along linedef to start of seg
- Angle
- Pointer to sidedef
- Pointer to linedef

### Subsector

References a Sector. List of segs

Data Structure:

- Number of segs
- Seg number of first seg
  - Seg numbers in this subsector are first seg through first seg + number of segs - 1

### Reject

### Player

Special Thing (Map Object)

Data Structure:

- Mob Object
- View height
- Base view height

### Rendering State



## ~~Raycasting~~

Doom doesn't use raycasting for rendering, instead using Binary Space Partitioning

## Binary Space Partitioning

Binary Space Paritioning, or BSP, divides a level into a binary tree, with nodes being an area of the level. Each branch divides the previous area into two subnodes, with the dividing lines being segs, or line segments. The leaves of this tree are convex polygons called subsectors. This is all done during level loading, not during gameplay.

Once the level has been divided into this binary tree, it is used to decide which subsector to render first by binary searching for the subsector the player is in.
