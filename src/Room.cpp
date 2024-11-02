



#include "Room.hpp"





/// -----------------------      Room     ----------------------------------------



Room::Room() :
      neighbors(),
      walls(),
      path_set(0),
      xyzcoord()
{
   Reset();
}



void Room::Reset() {
   for (unsigned int i = 0 ; i < NUM_ROOM_FACES ; ++i) {
      neighbors[i] = 0;
   }
   for (unsigned int i = 0 ; i < NUM_ROOM_FACES ; ++i) {
      walls[i] = 0;
   }
   path_set = 0;
   xyzcoord.SetXYZ(0,0,0);
}



void Room::SetNeighbor(ROOM_FACE room_dir , Room* r) {
   EAGLE_ASSERT(room_dir >=0 && room_dir < NUM_ROOM_FACES);
   neighbors[room_dir] = r;
}



void Room::SetWall(ROOM_FACE room_dir , Wall* w) {
   walls[room_dir] = w;
}


void Room::SetPathSet(PathSet* ps) {
   path_set = ps;
}
