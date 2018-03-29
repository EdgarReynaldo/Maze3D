



#include "Room.hpp"





/// -----------------------      Room     ----------------------------------------



Room::Room() :
      faces(),
      path_set(0)
{
   Reset();
}



void Room::Reset() {
   memset(faces , 0 , sizeof(Face*)*NUM_ROOM_FACES);
}



void Room::SetFace(ROOM_FACE room_dir , Face* face) {
   faces[room_dir] = face;
}


void Room::SetPathSet(PathSet* ps) {
   path_set = ps;
}
