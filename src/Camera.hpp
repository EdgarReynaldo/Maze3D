


#ifndef Camera_HPP
#define Camera_HPP


#include "Vec3D.hpp"


#include "GL/gl.h"
#include "GL/glu.h"


class Camera {
public :
   Vec3D pos;
   Vec3D look;
   Vec3D right;
   Vec3D up;
   
   Camera();
   void SetupView();
   void SetupSkyboxView();
   
   void SetPosition(double x , double y , double z);
   void SetPosition(const Vec3D& v);
   void LookAt(double x , double y , double z);
   
   void DrawAxes(float axis_length);
   
};



Vec3D OrbitOrigin(double distance , double yaxisrotation , double ascension);/// Pass degrees (0-360) , (-90,90)




#endif // Camera_HPP





