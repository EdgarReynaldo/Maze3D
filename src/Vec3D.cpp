


#include "Vec3D.hpp"


#include <cmath>



/// -----------------------------     Vec3D     ---------------------------------




Vec3D::Vec3D() :
      x(0.0),
      y(0.0),
      z(0.0)
{}



Vec3D::Vec3D(double xpos , double ypos , double zpos) : 
      x(xpos),
      y(ypos),
      z(zpos)
{}



void Vec3D::SetXYZ(double xpos , double ypos , double zpos) {
   x = xpos;
   y = ypos;
   z = zpos;
}



Vec3D Vec3D::operator+(const Vec3D& v) {
   x += v.x;
   y += v.y;
   z += v.z;
   return *this;
}



Vec3D Vec3D::operator-(const Vec3D& v) {
   return operator+(-v);
}



Vec3D Vec3D::operator-() const {
   return Vec3D(-x,-y,-z);
}



Vec3D Vec3D::operator*(double factor) {
   x *= factor;
   y *= factor;
   z *= factor;
   return *this;
}



Vec3D Vec3D::operator/(double quotient) {
   return operator*(1.0/quotient);
}



double Vec3D::Magnitude() {
   return sqrt(x*x + y*y + z*z);
}



Vec3D& Vec3D::Normalize() {
   *this = *this/Magnitude();
   return *this;
}

   
   
   
   
