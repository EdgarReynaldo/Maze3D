



#ifndef Room_HPP
#define Room_HPP


#include "Face.hpp"
#include "Eagle/Mesh.hpp"
#include "Eagle/Vec3.hpp"



class PathSet;


class Room {
friend class Maze;
   
//   Face* faces[NUM_ROOM_FACES];/// Inside faces of room
   Room* neighbors[NUM_ROOM_FACES];
   Wall* walls[NUM_ROOM_FACES];
   PathSet* path_set;/// holds a list of rooms that can be reached by this room
   Vec3 xyzcoord;
public :

   Room();

   void Reset();

   void SetNeighbor(ROOM_FACE room_dir , Room* r);
   void SetWall(ROOM_FACE room_dir , Wall* w);
   void SetPathSet(PathSet* ps);
   
   PathSet* GetPathSet() {return path_set;}
};



#endif // Room_HPP
