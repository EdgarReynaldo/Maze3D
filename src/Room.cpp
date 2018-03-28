



#include "Room.hpp"





/// -----------------------      Room     ----------------------------------------



void Room::SetFace(ROOM_FACE room_dir , Face* face) {
   faces[room_dir] = face;
}


