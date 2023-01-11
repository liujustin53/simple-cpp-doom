#include "wad/WadFile.hpp"

#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "game/Player.hpp"
#include "misc/BoundingBox.hpp"
#include "render/Render.hpp"
#include "render/RenderState.hpp"

namespace wad {
WadFile::WadFile(const std::string& path) {
  std::cout << "Loading wad file: " << path << std::endl;
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::invalid_argument("Could not open wad file.");
  }

  // read the entire file into a vector of lines
  std::vector<std::string> lines;
  std::string line;
  while (std::getline(file, line)) {
    lines.push_back(line);
  }

  // close the file
  file.close();

  // read the header of the wad file
  std::string header = lines[0];
  if (header != "WAD" && header != "IWAD" && header != "PWAD") {
    throw std::invalid_argument("Invalid wad file.");
  }

  // read the number of lumps and the offset to the directory
  // the offset is 1-indexed
  int num_lumps, dir_offset;
  std::istringstream(lines[1]) >> num_lumps >> dir_offset;
  dir_offset--;

  try {
    // read the directory
    for (int i = 0; i < num_lumps; i++) {
      // read a directory entry
      std::string entry = lines[dir_offset + i];
      std::istringstream entry_stream(entry);

      // read the name of the lump
      std::string lump_name;
      entry_stream >> lump_name;

      // read the offset of the lump (1-indexed)
      int lump_offset;
      entry_stream >> lump_offset;
      lump_offset--;

      // read the size of the lump
      int lump_size;
      entry_stream >> lump_size;

      // read the lump
      WadLump lump;
      for (int j = 0; j < lump_size; j++) {
        lump.push_back(lines[lump_offset + j]);
      }

      // add the lump to the map
      lumps[lump_name] = lump;
    }
  } catch (std::invalid_argument& e) {
    throw std::invalid_argument("Invalid wad file. Error: " +
                                std::string(e.what()));
  }
}

void WadFile::loadRenderState(render::RenderState& state) {
  // create the vertices
  for (std::string line : lumps["VERTEXES"]) {
    try {
      std::istringstream line_stream(line);
      int x, y;
      line_stream >> x >> y;

      // check if the coordinates are valid
      if (x < 0 || y < 0) {
        throw std::invalid_argument(
            "Invalid vertex coordinates, must be non-negative, are (" +
            std::to_string(x) + ", " + std::to_string(y) + ").");
      }

      state.vertexes.push_back(new render::Vertex(x, y));
    } catch (std::invalid_argument& e) {
      throw std::invalid_argument("Invalid vertex at line: " + line +
                                  ". Error: " + std::string(e.what()));
    }
  }

  // create the sectors
  for (std::string line : lumps["SECTORS"]) {
    try {
      // read the floor and ceiling heights
      std::istringstream line_stream(line);
      int floor_height, ceiling_height;
      line_stream >> floor_height >> ceiling_height;

      // read the floor and ceiling colors
      sf::Color floor_color, ceiling_color;
      line_stream >> floor_color.r >> floor_color.g >> floor_color.b;
      line_stream >> ceiling_color.r >> ceiling_color.g >> ceiling_color.b;

      // read the light levels
      std::array<int, 5> light_levels;
      for (size_t i = 0; i < 5; i++) {
        int brightness;
        line_stream >> brightness;
        // check if the brightness is valid
        if (brightness < -255 || brightness > 255) {
          throw std::invalid_argument(
              "Invalid brightness value, must be between -255 and 255, is " +
              std::to_string(brightness) + ".");
        }
        light_levels[i] = brightness;
      }

      state.sectors.push_back(new render::Sector(floor_height, ceiling_height,
                                                 floor_color, ceiling_color,
                                                 light_levels));
    } catch (const std::exception& e) {
      throw std::invalid_argument("Invalid sector at line: " + line +
                                  ". Error: " + std::string(e.what()));
    }
  }

  // create the sidedefs
  for (std::string line : lumps["SIDEDEFS"]) {
    try {
      // read the three colors
      std::istringstream line_stream(line);
      int RGBs[3][3];
      for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
          line_stream >> RGBs[i][j];
          // check if the color is valid
          if (RGBs[i][j] < 0 || RGBs[i][j] > 255) {
            throw std::invalid_argument(
                "Invalid color value, must be between 0 and 255, is " +
                std::to_string(RGBs[i][j]) + ".");
          }
        }
      }
      sf::Color upper_color(RGBs[0][0], RGBs[0][1], RGBs[0][2]);
      sf::Color lower_color(RGBs[1][0], RGBs[1][1], RGBs[1][2]);
      sf::Color middle_color(RGBs[2][0], RGBs[2][1], RGBs[2][2]);

      // read the sector index
      int sector_index;
      line_stream >> sector_index;

      // get the sector the sidedef is in
      render::Sector* sector;
      try {
        sector = state.sectors.at(sector_index - 1);
      } catch (const std::out_of_range& e) {
        throw std::invalid_argument(
            "Invalid sector index, sector: " + std::to_string(sector_index) +
            " does not exist");
      }
      // check if the sector index is valid
      if (sector == nullptr) {
        throw std::invalid_argument(
            "Invalid sector index, sector: " + std::to_string(sector_index) +
            " does not exist");
      }

      state.sidedefs.push_back(
          new render::SideDef(upper_color, lower_color, middle_color, sector));
    } catch (const std::exception& e) {
      throw std::invalid_argument("Invalid sidedef at line: " + line +
                                  ". Error: " + std::string(e.what()));
    }
  }

  // create the linedefs
  for (std::string line : lumps["LINEDEFS"]) {
    try {
      // read the two vertex indices
      std::istringstream line_stream(line);
      int vertex1_index, vertex2_index;
      line_stream >> vertex1_index >> vertex2_index;

      // get the first vertex
      render::Vertex* vertex1;
      try {
        vertex1 = state.vertexes.at(vertex1_index - 1);
      } catch (const std::out_of_range& e) {
        throw std::invalid_argument(
            "Invalid vertex index, vertex1: " + std::to_string(vertex1_index) +
            " does not exist");
      }

      // get the second vertex
      render::Vertex* vertex2;
      try {
        vertex2 = state.vertexes.at(vertex2_index - 1);
      } catch (const std::out_of_range& e) {
        throw std::invalid_argument(
            "Invalid vertex index, vertex2: " + std::to_string(vertex2_index) +
            " does not exist");
      }

      // check if the linedef is two-sided
      int two_sided;
      line_stream >> two_sided;
      if (two_sided != 0 && two_sided != 1) {
        throw std::invalid_argument(
            "Invalid two-sided value, must be 0 or 1, is " +
            std::to_string(two_sided) + ".");
      }

      // read the front sidedef index
      int front_sidedef_index, back_sidedef_index;
      line_stream >> front_sidedef_index;

      // get the front sidedef
      render::SideDef* front_sidedef;
      try {
        front_sidedef = state.sidedefs.at(front_sidedef_index - 1);
      } catch (const std::out_of_range& e) {
        throw std::invalid_argument(
            "Invalid front sidedef index, front sidedef: " +
            std::to_string(front_sidedef_index) + " does not exist");
      }

      // get the back sidedef if the linedef is two-sided
      render::SideDef* back_sidedef = nullptr;
      if (two_sided) {
        line_stream >> back_sidedef_index;
        try {
          back_sidedef = state.sidedefs.at(back_sidedef_index - 1);
        } catch (const std::out_of_range& e) {
          throw std::invalid_argument(
              "Invalid back sidedef index, back sidedef: " +
              std::to_string(back_sidedef_index) + " does not exist");
        }
      }

      // create the linedef
      render::LineDef* linedef =
          new render::LineDef(vertex1, vertex2, front_sidedef, back_sidedef);

      // add the linedef to the sectors it belongs to
      linedef->sidedef[0]->sector->addLineDef(linedef);
      if (linedef->sidedef[1] != nullptr) {
        linedef->sidedef[1]->sector->addLineDef(linedef);
      }

      state.linedefs.push_back(linedef);
    } catch (const std::exception& e) {
      throw std::invalid_argument("Invalid linedef at line: " + line +
                                  ". Error: " + std::string(e.what()));
    }
  }

  // create the segs
  for (std::string line : lumps["SEGS"]) {
    try {
      // read the two vertex indices
      std::istringstream line_stream(line);
      int vertex1_index, vertex2_index;
      line_stream >> vertex1_index >> vertex2_index;

      // get the first vertex
      render::Vertex* vertex1;
      try {
        vertex1 = state.vertexes.at(vertex1_index - 1);
      } catch (const std::out_of_range& e) {
        throw std::invalid_argument(
            "Invalid vertex index, vertex1: " + std::to_string(vertex1_index) +
            " does not exist");
      }

      // get the second vertex
      render::Vertex* vertex2;
      try {
        vertex2 = state.vertexes.at(vertex2_index - 1);
      } catch (const std::out_of_range& e) {
        throw std::invalid_argument(
            "Invalid vertex index, vertex2: " + std::to_string(vertex2_index) +
            " does not exist");
      }

      // read the linedef index
      int linedef_index;
      line_stream >> linedef_index;

      // get the linedef
      render::LineDef* linedef;
      try {
        linedef = state.linedefs.at(linedef_index - 1);
      } catch (const std::out_of_range& e) {
        throw std::invalid_argument(
            "Invalid linedef index, linedef: " + std::to_string(linedef_index) +
            " does not exist");
      }

      // read the direction
      int direction;
      line_stream >> direction;
      if (direction != 0 && direction != 1) {
        throw std::invalid_argument(
            "Invalid direction value, must be 0 or 1, is " +
            std::to_string(direction) + ".");
      }

      // get the sidedef
      render::SideDef* sidedef = linedef->sidedef[direction];

      // check if the sidedef exists
      if (sidedef == nullptr) {
        throw std::invalid_argument(
            "Invalid direction value, there is no sidedef on side " +
            std::to_string(direction) + ".");
      }

      state.segs.push_back(new render::Seg(vertex1, vertex2, linedef, sidedef));
    } catch (const std::exception& e) {
      throw std::invalid_argument("Invalid seg at line: " + line +
                                  ". Error: " + std::string(e.what()));
    }
  }

  // create the subsectors
  for (std::string line : lumps["SSECTORS"]) {
    try {
      // read the index of the first seg
      std::istringstream line_stream(line);
      int first_seg_index;
      line_stream >> first_seg_index;

      // read the number of segs
      int num_segs;
      line_stream >> num_segs;

      // get the first seg
      render::Seg* first_seg;
      try {
        first_seg = state.segs.at(first_seg_index - 1);
      } catch (const std::out_of_range& e) {
        throw std::invalid_argument(
            "Invalid seg index, first seg: " + std::to_string(first_seg_index) +
            " does not exist");
      }
      // get the sector of the first seg
      render::Sector* sector = first_seg->sidedef->sector;

      // get all the segs
      std::vector<render::Seg*> segs;
      segs.push_back(first_seg);
      for (int i = 1; i < num_segs; i++) {
        render::Seg* seg;
        try {
          // get the seg
          seg = state.segs.at(first_seg_index + i - 1);
        } catch (const std::out_of_range& e) {
          throw std::invalid_argument(
              "Invalid seg index, seg: " + std::to_string(first_seg_index + i) +
              " does not exist");
        }
        // check if the seg is in the same sector as the first seg
        if (seg->sidedef->sector != sector) {
          throw std::invalid_argument(
              "Seg " + std::to_string(first_seg_index + i) +
              " is not in the same sector as the first seg: " +
              std::to_string(first_seg_index) + ".");
        }
        segs.push_back(seg);
      }
      state.subsectors.push_back(new render::Subsector(segs));
    } catch (const std::exception& e) {
      throw std::invalid_argument("Invalid subsector at line: " + line +
                                  ". Error: " + std::string(e.what()));
    }
  }

  // create the nodes
  for (std::string line : lumps["NODES"]) {
    try {
      // read the x y dx and dy of the partition line
      std::istringstream line_stream(line);
      int x, y, dx, dy;
      line_stream >> x >> y >> dx >> dy;

      // read the right and left bounding boxes
      std::array<int, 4> right_bbox;
      std::array<int, 4> left_bbox;
      for (int i = 0; i < 4; i++) {
        line_stream >> right_bbox[i];
      }
      for (int i = 0; i < 4; i++) {
        line_stream >> left_bbox[i];
      }

      // create the right and left bounding boxes
      misc::BoundingBox right_bounding_box(right_bbox);
      misc::BoundingBox left_bounding_box(left_bbox);

      // read the right and left child indices
      int right_child_index, left_child_index;
      line_stream >> right_child_index >> left_child_index;

      state.nodes.push_back(
          new render::Node(x, y, dx, dy, right_bounding_box, left_bounding_box,
                           right_child_index, left_child_index));
    } catch (const std::exception& e) {
      throw std::invalid_argument("Invalid node at line: " + line +
                                  ". Error: " + std::string(e.what()));
    }
  }

  // find the player in the things lump
  for (std::string line : lumps["THINGS"]) {
    std::istringstream line_stream(line);
    int x, y, z, angle, type;
    line_stream >> x >> y >> z >> angle >> type;
    if (type == 1) {
      state.player = new game::Player(x, y, z, angle);
      return;
    }
  }
  // if the player was not found, throw an exception
  throw std::invalid_argument("Could not find player in wad file.");
}

}  // namespace wad