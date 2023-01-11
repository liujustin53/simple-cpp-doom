#ifndef MOBJ_HPP
#define MOBJ_HPP

namespace render {
class Subsector;
}

namespace logic {

class MObj {
  // TODO: make private
 public:
  int x;
  int y;
  int z;
  int angle;

  // subsector this mobj is in
  render::Subsector* subsector;

 public:
  MObj(int x, int y, int z, int angle);
  void moveToSubsector(render::Subsector* subsector);
  render::Subsector* getSubsector();
};

}  // namespace logic

#endif