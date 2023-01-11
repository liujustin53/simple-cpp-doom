#ifndef WADFILE_HPP
#define WADFILE_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace render {
class RenderState;
}  // namespace render

namespace wad {

typedef std::vector<std::string> WadLump;

class WadFile {
  std::unordered_map<std::string, WadLump> lumps;

 public:
  WadFile(const std::string& path);

  // load the contents of the wad file into a render state
  void loadRenderState(render::RenderState& state);
};

}  // namespace wad

#endif