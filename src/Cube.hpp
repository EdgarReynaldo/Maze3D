



#ifndef Cube_HPP
#define Cube_HPP




#include "Eagle/Vec3.hpp"
#include "Eagle/Texture.hpp"


class EagleImage;



enum CUBEFACE {
   CUBEFRONT = 0,
   CUBERIGHT = 1,
   CUBEBACK = 2,
   CUBELEFT = 3,
   CUBETOP = 4,
   CUBEBOTTOM = 5,
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
   Vec3 p1,p2;
   
   CubeEdge();
   CubeEdge(Vec3 a , Vec3 b);
   void Render();

};

#define NUM_CUBE_EDGES 12



class CubeFace {
public :
   Vec3 corners[NUM_CUBE_CORNERS];
   TEXTEX texcorners[NUM_CUBE_CORNERS];

   CubeFace();
   CubeFace(Vec3 tl , Vec3 bl , Vec3 br , Vec3 tr , bool wind_ccw);
   
   void SetCorners(Vec3 tl , Vec3 bl , Vec3 br , Vec3 tr , bool wind_ccw);
   
///   void Render();
};



class Cube {
public :
   Vec3    cube_verts    [NUM_CUBE_VERTS];
   CubeFace cube_faces_out[NUM_CUBE_FACES];
   CubeFace cube_faces_in [NUM_CUBE_FACES];
   CubeEdge cube_edges    [NUM_CUBE_EDGES];
   
   bool render_inside;
   bool render_outside;
   
   
   
   Cube();
   Cube(double size);
   
   void SetSize(double size);
   
   void SetTexture(CUBEFACE cf , EagleImage* img);
///   void Render();

};


#endif // Cube_HPP




