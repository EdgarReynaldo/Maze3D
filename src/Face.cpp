



#include "Face.hpp"


#include "Eagle/Vec3.hpp"



FACE_TYPE GetFaceType(ROOM_FACE rf) {
   return (FACE_TYPE)((int)rf/2);
}



ROOM_DIRECTION GetRoomDirection(ROOM_FACE rf) {
   return (ROOM_DIRECTION)((int)rf % 2);
}



/// --------------------     Face      ----------------------------



Face::Face(Wall* parent) :
      parent_wall(parent),
      v(),
      texid((GLuint(-1)))
{
}



void Face::Reset() {
   /// parent_wall = ... Don't assign here
   for (unsigned int i = 0 ; i < NUM_FACE_CORNERS ; ++i) {
      v[i] = 0;//.SetXYZ(0.0,0.0,0.0);
   }
   texid = (GLuint)-1;
}



void Face::SetVertex(FACE_CORNER corner , Vec3* vtx) {
   v[corner] = vtx;
}



void Face::Display() {
   
   Vec3 v3;
   glBindTexture(GL_TEXTURE_2D , texid);
   
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
   



   
