



#ifndef Cube_HPP
#define Cube_HPP




#include "Vec3D.hpp"
#include "Texture.hpp"

#include "GL/gl.h"


enum CUBEFACE {
   FRONT = 0,
   RIGHT = 1,
   BACK = 2,
   LEFT = 3,
   TOP = 4,
   BOTTOM = 5,
   NUM_CUBE_FACES = 6
};

enum CUBEVERTS {
   LBR = 0, /// left bottom rear
   LBF = 1, /// left bottom front
   LTR = 2, /// left top rear
   LTF = 3, /// left top front
   RBR = 4, /// right bottom rear
   RBF = 5, /// right bottom front
   RTR = 6, /// right top rear
   RTF = 7, /// right top front
   NUM_CUBE_VERTS = 8
};

enum CUBECORNER {
   CTL = 0,
   CBL = 1,
   CBR = 2,
   CTR = 3,
   NUM_CUBE_CORNERS = 4
};



class CubeEdge {
public :
   Vec3D p1,p2;
   
   CubeEdge();
   CubeEdge(Vec3D a , Vec3D b);
   void Render();

};

#define NUM_CUBE_EDGES 12



class CubeFace {
public :
   Vec3D corners[NUM_CUBE_CORNERS];
   Tex2D texcorners[NUM_CUBE_CORNERS];
   GLuint texid;
   
   CubeFace();
   CubeFace(Vec3D tl , Vec3D bl , Vec3D br , Vec3D tr , bool wind_ccw);
   
   void SetCorners(Vec3D tl , Vec3D bl , Vec3D br , Vec3D tr , bool wind_ccw);
   
///   void Render();
};



class Cube {
public :
   Vec3D    cube_verts    [NUM_CUBE_VERTS];
   CubeFace cube_faces_out[NUM_CUBE_FACES];
   CubeFace cube_faces_in [NUM_CUBE_FACES];
   CubeEdge cube_edges    [NUM_CUBE_EDGES];
   
   bool render_inside;
   bool render_outside;
   
   
   
   Cube();
   Cube(double size);
   
   void SetSize(double size);
   
   void SetTexture(CUBEFACE cf , GLuint id);
///   void Render();

};


#endif // Cube_HPP




