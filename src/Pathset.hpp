



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
   
   bool HasRoom(Room* r);
   bool AddRoom(Room* r);

   void AbsorbPathSet(PathSet* p);
};


#endif // Path_HPP

