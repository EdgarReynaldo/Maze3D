



#ifndef Room_HPP
#define Room_HPP


#include "Face.hpp"

class Room {
   Face* faces[NUM_ROOM_FACES];

public :

///   Room();
   Room() :
         faces()
   {
      memset(faces , 0 , sizeof(Face*)*NUM_ROOM_FACES);
   }


   void SetFace(ROOM_FACE room_dir , Face* face);

};



#endif // Room_HPP
