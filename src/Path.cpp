



#include "Path.hpp"




bool PathSet::HasRoom(Room* r) {
   return rooms.find(r) != rooms.end();
}
   
}



bool PathSet::AddRoom(Room* r) {
   if (!HasRoom(r)) {
      rooms.insert(r);
      return true;
   }
   return false;
}



void PathSet::AbsorbPathSet(PathSet* p) {
   rooms.insert(p->begin() , p->end());
}
