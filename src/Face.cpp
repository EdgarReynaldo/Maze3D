



#include "Face.hpp"


#include "Eagle/Vec3.hpp"



ROOM_FACE OppositeFace(ROOM_FACE f) {
   if (f < NUM_ROOM_FACES && f >= 0) {
      if (f % 2 == 0) {
         return (ROOM_FACE)((int)f + 1);}
      else {
         return (ROOM_FACE)((int)f - 1);
      }
   }
   return NUM_ROOM_FACES;
}



FACE_TYPE GetFaceType(ROOM_FACE rf) {
   return (FACE_TYPE)((int)rf/2);
}



ROOM_DIRECTION GetRoomDirection(ROOM_FACE rf) {
   return (ROOM_DIRECTION)((int)rf % 2);
}



/// --------------------     Face      ----------------------------



void Face::ResetNormal() {
   
   Vec3 veca = *v[FC_UPPERLEFT] - *v[FC_LOWERLEFT];
   Vec3 vecb = *v[FC_LOWERRIGHT] - *v[FC_LOWERLEFT];
   normal = CrossProduct(veca , vecb);
}



Face::Face(Wall* parent) :
      parent_wall(parent),
      v(),
      normal(),
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
///   ResetNormal();// Not all corners may be valid yet
}



void Face::Display() {
   
   Vec3 v3;
   glBindTexture(GL_TEXTURE_2D , texid);
   
   glBegin(GL_TRIANGLE_FAN);
      glColor3ub(255,255,255);
      
      v3 = *v[FC_UPPERLEFT];
      glTexCoord2f(0.0f , 1.0f);
      glNormal3f(normal.x , normal.y , normal.z);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_LOWERLEFT];
      glTexCoord2f(0.0f , 0.0f);
      glNormal3f(normal.x , normal.y , normal.z);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_LOWERRIGHT];
      glTexCoord2f(1.0f , 0.0f);
      glNormal3f(normal.x , normal.y , normal.z);
      glVertex3d(v3.x , v3.y , v3.z);

      v3 = *v[FC_UPPERRIGHT];
      glTexCoord2f(1.0f , 1.0f);
      glNormal3f(normal.x , normal.y , normal.z);
      glVertex3d(v3.x , v3.y , v3.z);
      
   glEnd();
}


void Face::Outline(EagleColor col) {
   
   Vec3 v3;
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
   



   
