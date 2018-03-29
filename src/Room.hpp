



#ifndef Room_HPP
#define Room_HPP


#include "Face.hpp"

class PathSet;

class Room {
   Face* faces[NUM_ROOM_FACES];
   PathSet* path_set;/// holds a list of rooms that can be reached by this room
   
public :

   Room();

   void Reset();

   void SetFace(ROOM_FACE room_dir , Face* face);
   void SetPathSet(PathSet* ps);
};



#endif // Room_HPP
