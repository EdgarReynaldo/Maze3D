


#include "Camera.hpp"

#include "Globals.hpp"
#include <cmath>



Camera::Camera() :
      pos(0,0,-world_size/2.0),
      look(0,0,1),
      right(1,0,0),
      up(0,1,0)
{}



void Camera::SetupView() {
   /// Perspective matrix
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60.0 , (float)ww/wh , 1.0 , world_size);

   /// View matrix
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(pos.x , pos.y , pos.z , pos.x + look.x , pos.y + look.y , pos.z + look.z , up.x , up.y , up.z);
}



void Camera::SetupSkyboxView() {
   /// Perspective matrix
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(60 , (float)ww/wh , 1 , world_size);
   
   /// View matrix
   /// For our skybox, we always want to render as if we were looking in the same direction, but from the origin
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   gluLookAt(0 , 0 , 0 , look.x , look.y , look.z , up.x , up.y , up.z);
}



void Camera::SetPosition(double x , double y , double z) {
   pos.SetXYZ(x,y,z);
}



void Camera::SetPosition(const Vec3D& v) {
   pos = v;
}



void Camera::LookAt(double x , double y , double z) {
   Vec3D viewed(x,y,z);
   look = (viewed - pos).Normalize();
}



void Camera::DrawAxes(float axis_length) {
   float d = axis_length;
   glBegin(GL_LINES);
   glColor3f(1.0f , 0.0f , 0.0f);
   glVertex3f(0.0f , 0.0f , 0.0f);
   glVertex3f(   d , 0.0f , 0.0f);
   glColor3f(0.0f , 1.0f , 0.0f);
   glVertex3f(0.0f , 0.0f , 0.0f);
   glVertex3f(0.0f ,    d , 0.0f);
   glColor3f(0.0f , 0.0f , 1.0f);
   glVertex3f(0.0f , 0.0f , 0.0f);
   glVertex3f(0.0f , 0.0f ,    d);
   glEnd();
}



Vec3D OrbitOrigin(double distance , double yaxisrotation , double ascension) {
   /// sine(ascension) = yheight/distance;
   double y = distance*sin(ascension*M_PI/180.0);
   double proj = distance*cos(ascension*M_PI/180.0);
   double z = proj*sin(yaxisrotation*M_PI/180.0);
   double x = proj*cos(yaxisrotation*M_PI/180.0);
   return Vec3D(x,y,z);
}
