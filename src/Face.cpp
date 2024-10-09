



#include "Face.hpp"


#include "Eagle/Vec3.hpp"



FACE_TYPE GetFaceType(ROOM_FACE rf) {
   return (FACE_TYPE)((int)rf/2);
}



ROOM_DIRECTION GetRoomDirection(ROOM_FACE rf) {
   return (ROOM_DIRECTION)((int)rf % 2);
}



/// --------------------     Face      ----------------------------



Face::Face() :
      reverse_face(0),
      rooms(),
      v(),
      texid((unsigned int)-1),
      kweight(0),
      open(false)
{
   Reset();
}



void Face::Reset() {
   reverse_face = 0;
   memset(rooms , 0 , sizeof(Room*)*ROOM_NUM_DIRECTIONS);
   memset(v , 0 , sizeof(Vec3*)*NUM_FACE_CORNERS);
   texid = (unsigned int)-1;
   kweight = 0;
   open = false;
}



void Face::SetRoom(ROOM_DIRECTION dir , Room* room) {
   rooms[dir] = room;
}



void Face::SetVertex(FACE_CORNER corner , Vec3* vtx) {
   v[corner] = vtx;
}



void Face::SetWeight(int w) {
   kweight = w;
}



void Face::Display(GLuint tex) {
   if (open) {return;}
   Vec3 v3;
   glBindTexture(GL_TEXTURE_2D , tex);
   
   glBegin(GL_TRIANGLE_FAN);
      glColor3ub(255,255,255);
      
      v3 = *v[FC_UPPERLEFT];
      glTexCoord2f(0.0f , 1.0f);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_LOWERLEFT];
      glTexCoord2f(0.0f , 0.0f);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_LOWERRIGHT];
      glTexCoord2f(1.0f , 0.0f);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_UPPERRIGHT];
      glTexCoord2f(1.0f , 1.0f);
      glVertex3d(v3.x , v3.y , v3.z);
      
   glEnd();
}


void Face::Outline(EagleColor col) {
   
   Vec3 v3;
   glDisable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_CULL_FACE);
   glBegin(GL_LINE_LOOP);
      glColor4i(col.r , col.g , col.b , col.a);
      
      v3 = *v[FC_UPPERLEFT];
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_LOWERLEFT];
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_LOWERRIGHT];
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_UPPERRIGHT];
      glVertex3d(v3.x , v3.y , v3.z);
   glEnd();
}
   


Room* Face::GetRoom(ROOM_DIRECTION dir) {
   return rooms[dir];
}
