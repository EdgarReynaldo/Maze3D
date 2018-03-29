



#ifndef Path_HPP
#define Path_HPP

#include <hash_set>

class Room;

class PathSet {
   
protected:
   
   std::hash_set<Room*> rooms;

public :
      
   PathSet();
   PathSet() :
         rooms()
   {}
   
   bool HasRoom(Room* r);
   bool AddRoom(Room* r);

   void AbsorbPathSet(PathSet* p);
};


#endif // Path_HPP

