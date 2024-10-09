



#ifndef Room_HPP
#define Room_HPP


#include "Face.hpp"
#include "Eagle/Mesh.hpp"

class PathSet;


class Room {
   Face* faces[NUM_ROOM_FACES];/// Inside faces of room
   PathSet* path_set;/// holds a list of rooms that can be reached by this room
   
public :

   Room();

   void Reset();

   void SetFace(ROOM_FACE room_dir , Face* face);
   void SetPathSet(PathSet* ps);
   
   PathSet* GetPathSet() {return path_set;}
};



#endif // Room_HPP
