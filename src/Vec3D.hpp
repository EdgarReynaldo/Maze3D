



#ifndef Vert_HPP
#define Vert_HPP


class Vec3D {
public :
   double x,y,z;
   
   Vec3D();
   Vec3D(double xpos , double ypos , double zpos);
   
   void SetXYZ(double xpos , double ypos , double zpos);
   
   Vec3D operator+(const Vec3D& v);
   Vec3D operator-(const Vec3D& v);
   Vec3D operator-() const;
   Vec3D operator*(double factor);
   Vec3D operator/(double quotient);

   double Magnitude();
   Vec3D& Normalize();
};



#endif // Vert_HPP

