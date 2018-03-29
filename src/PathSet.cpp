



#include "PathSet.hpp"




void PathSet::Reset() {
   rooms.clear();
}



bool PathSet::HasRoom(Room* r) {
   return rooms.find(r) != rooms.end();
}
   


bool PathSet::AddRoom(Room* r) {
   if (!HasRoom(r)) {
      rooms.insert(r);
      return true;
   }
   return false;
}



void PathSet::AbsorbPathSet(PathSet* p) {
   rooms.insert(p->rooms.begin() , p->rooms.end());
}
