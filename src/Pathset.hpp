



#ifndef Path_HPP
#define Path_HPP

#include <unordered_set>

class Room;

class PathSet {
   
protected:
   
   std::unordered_set<Room*> rooms;

public :
      
///   PathSet();
   PathSet() :
         rooms()
   {}
   
   void Reset();
   
   void Clear();
   
   bool HasRoom(Room* r);
   bool AddRoom(Room* r);

   void SetAllRoomsPathSet(PathSet* pset);
   
   void AbsorbPathSet(PathSet* pset_absorb);
};


#endif // Path_HPP

