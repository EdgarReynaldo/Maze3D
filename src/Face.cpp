



#include "Face.hpp"



FACE_TYPE GetFaceType(ROOM_FACE rf) {
   return (FACE_TYPE)((int)rf/2);
}



ROOM_DIRECTION GetRoomDirection(ROOM_FACE rf) {
   return (ROOM_DIRECTION)((int)rf % 2);
}



/// --------------------     Face      ----------------------------



Face::Face() :
      rooms(),
      v(),
      texidpos((unsigned int)-1),
      texidneg((unsigned int)-1),
      kweight(0),
      open(false)
{
   Reset();
}



void Face::Reset() {
   memset(rooms , 0 , sizeof(Room*)*ROOM_NUM_DIRECTIONS);
   memset(v , 0 , sizeof(Vec3D*)*NUM_FACE_CORNERS);
   texidpos = texidneg = (unsigned int)-1;
   kweight = 0;
   open = false;
}



void Face::SetRoom(ROOM_DIRECTION dir , Room* room) {
   rooms[dir] = room;
}



void Face::SetVertex(FACE_CORNER corner , Vec3D* vtx) {
   v[corner] = vtx;
}



void Face::SetWeight(int w) {
   kweight = w;
}



Room* Face::GetRoom(ROOM_DIRECTION dir) {
   return rooms[dir];
}
