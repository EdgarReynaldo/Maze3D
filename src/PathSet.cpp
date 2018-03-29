



#include "PathSet.hpp"
#include "Room.hpp"

#include <cassert>

void PathSet::Reset() {
   Clear();
}



void PathSet::Clear() {
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



void PathSet::SetAllRoomsPathSet(PathSet* pset) {
   std::unordered_set<Room*>::iterator it = rooms.begin();
   while (it != rooms.end()) {
      Room* r = *it;
      r->SetPathSet(pset);
      ++it;
   }
}



void PathSet::AbsorbPathSet(PathSet* pset_absorb) {
   assert(pset_absorb);
   
   pset_absorb->SetAllRoomsPathSet(this);/// Prevent invalidation of Room*s
   
   rooms.insert(pset_absorb->rooms.begin() , pset_absorb->rooms.end());/// Splice the two sets
   
   pset_absorb->Clear();
   
}




