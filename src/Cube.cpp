



#include "Cube.hpp"



/// ------------------------     CubeEdge     -----------------------------



CubeEdge::CubeEdge() :
      p1(),
      p2()
{}



CubeEdge::CubeEdge(Vec3D a , Vec3D b) :
      p1(a),
      p2(b)
{}



void CubeEdge::Render() {
   glBegin(GL_LINES);
   glColor3f(1.0f,1.0f,1.0f);
   glVertex3d(p1.x , p1.y , p1.z);
   glColor3f(1.0f,1.0f,1.0f);
   glVertex3d(p2.x , p2.y , p2.z);
   glEnd();
}





/// ---------------------      CubeFace     -----------------------------



CubeFace::CubeFace() :
      corners(),
      texcorners(),
      texid((unsigned int)-1)
{}



CubeFace::CubeFace(Vec3D tl , Vec3D bl , Vec3D br , Vec3D tr , bool wind_ccw) {
   SetCorners(tl,bl,br,tr,wind_ccw);
}



void CubeFace::SetCorners(Vec3D tl , Vec3D bl , Vec3D br , Vec3D tr , bool wind_ccw) {
   if (wind_ccw) {
      /// All coordinates have CCW winding
      corners[CTL] = tl;
      corners[CBL] = bl;
      corners[CBR] = br;
      corners[CTR] = tr;
      /// OpenGL texture coordinates, origin at bottom left, x goes + to the right and y goes + to the up
      /// These are upside down with regards to allegro
      texcorners[CTL] = {0.0 , 1.0};
      texcorners[CBL] = {0.0 , 0.0};
      texcorners[CBR] = {1.0 , 0.0};
      texcorners[CTR] = {1.0 , 1.0};
   }
   else {
      /// All coordinates have CW winding, simply reverse order
      corners[CTL] = tr;
      corners[CBL] = br;
      corners[CBR] = bl;
      corners[CTR] = tl;
      /// OpenGL texture coordinates, origin at bottom left, x goes + to the right and y goes + to the up
      /// Flip them left to right so they appear inside out
      texcorners[CTL] = {1.0 , 1.0};
      texcorners[CBL] = {1.0 , 0.0};
      texcorners[CBR] = {0.0 , 0.0};
      texcorners[CTR] = {0.0 , 1.0};
   }
}


/**
void CubeFace::Render() {
   Tex2D* tc = &(texcorners[0]);
   Vec3D* c = &(corners[0]);

   if (textures_on) {
      glBindTexture(GL_TEXTURE_2D , texid);
   }

///   glBegin(GL_QUADS);/// We can use QUADS or TRIANGLE_FAN here, due to the order and winding of the face
   glBegin(GL_TRIANGLE_FAN);
   for (int i = 0 ; i < 4 ; ++i) {
      if (textures_on) {
         glTexCoord2d(tc->u , tc->v);
      }
      glVertex3d(c->x , c->y , c->z);
      ++tc;
      ++c;
   }
   glEnd();
}
*/


/// ------------------------      Cube      --------------------------



Cube::Cube() :
      cube_verts(),
      cube_faces_out(),
      cube_faces_in(),
      cube_edges(),
      render_inside(true),
      render_outside(true)
{
   SetSize(1.0);
}



Cube::Cube(double size) :
      cube_verts(),
      cube_faces_out(),
      cube_faces_in(),
      cube_edges(),
      render_inside(true),
      render_outside(true)
{
   SetSize(size);
}



void Cube::SetSize(double size) {
   /// Creates a cube sized 'size X size X size'
   double side = size/2.0;
   cube_verts[LBR].SetXYZ(-side , -side , -side);
   cube_verts[LBF].SetXYZ(-side , -side ,  side);
   cube_verts[LTR].SetXYZ(-side ,  side , -side);
   cube_verts[LTF].SetXYZ(-side ,  side ,  side);
   cube_verts[RBR].SetXYZ( side , -side , -side);
   cube_verts[RBF].SetXYZ( side , -side ,  side);
   cube_verts[RTR].SetXYZ( side ,  side , -side);
   cube_verts[RTF].SetXYZ( side ,  side ,  side);
   
   /// Populate the faces of the cube with the vertices
   
   /// Note this produces the OUTside of a cube
   cube_faces_out[FRONT]  = CubeFace(cube_verts[LTF] , cube_verts[LBF] , cube_verts[RBF] , cube_verts[RTF] , true);
   cube_faces_out[BACK]   = CubeFace(cube_verts[RTR] , cube_verts[RBR] , cube_verts[LBR] , cube_verts[LTR] , true);
   cube_faces_out[LEFT]   = CubeFace(cube_verts[LTR] , cube_verts[LBR] , cube_verts[LBF] , cube_verts[LTF] , true);
   cube_faces_out[RIGHT]  = CubeFace(cube_verts[RTF] , cube_verts[RBF] , cube_verts[RBR] , cube_verts[RTR] , true);
   cube_faces_out[TOP]    = CubeFace(cube_verts[LTR] , cube_verts[LTF] , cube_verts[RTF] , cube_verts[RTR] , true);
   cube_faces_out[BOTTOM] = CubeFace(cube_verts[LBF] , cube_verts[LBR] , cube_verts[RBR] , cube_verts[RBF] , true);
   
   /// These are the exact same vertices, wound backwards (the INside of the cube)
   cube_faces_in[FRONT]  = CubeFace(cube_verts[LTF] , cube_verts[LBF] , cube_verts[RBF] , cube_verts[RTF] , false);
   cube_faces_in[BACK]   = CubeFace(cube_verts[RTR] , cube_verts[RBR] , cube_verts[LBR] , cube_verts[LTR] , false);
   cube_faces_in[LEFT]   = CubeFace(cube_verts[LTR] , cube_verts[LBR] , cube_verts[LBF] , cube_verts[LTF] , false);
   cube_faces_in[RIGHT]  = CubeFace(cube_verts[RTF] , cube_verts[RBF] , cube_verts[RBR] , cube_verts[RTR] , false);
   cube_faces_in[TOP]    = CubeFace(cube_verts[LTR] , cube_verts[LTF] , cube_verts[RTF] , cube_verts[RTR] , false);
   cube_faces_in[BOTTOM] = CubeFace(cube_verts[LBF] , cube_verts[LBR] , cube_verts[RBR] , cube_verts[RBF] , false);

   /// Populate the cube edges with vertices
   
   /// Front
   cube_edges[0]  = CubeEdge(cube_verts[LTF] , cube_verts[RTF]);
   cube_edges[1]  = CubeEdge(cube_verts[RTF] , cube_verts[RBF]);
   cube_edges[2]  = CubeEdge(cube_verts[RBF] , cube_verts[LBF]);
   cube_edges[3]  = CubeEdge(cube_verts[LBF] , cube_verts[LTF]);

   /// Back
   cube_edges[4]  = CubeEdge(cube_verts[LTR] , cube_verts[RTR]);
   cube_edges[5]  = CubeEdge(cube_verts[RTR] , cube_verts[RBR]);
   cube_edges[6]  = CubeEdge(cube_verts[RBR] , cube_verts[LBR]);
   cube_edges[7]  = CubeEdge(cube_verts[LBR] , cube_verts[LTR]);

   /// Sides
   cube_edges[8]  = CubeEdge(cube_verts[LTF] , cube_verts[LTR]);
   cube_edges[9]  = CubeEdge(cube_verts[RTF] , cube_verts[RTR]);
   cube_edges[10] = CubeEdge(cube_verts[RBF] , cube_verts[RBR]);
   cube_edges[11] = CubeEdge(cube_verts[LBF] , cube_verts[LBR]);
}



void Cube::SetTexture(CUBEFACE cf , GLuint id) {
   cube_faces_out[cf].texid = id;
   cube_faces_in[cf].texid = id;
}


/**
void Cube::Render() {

   /// Render faces
   if (model_on) {
      if (textures_on) {
         glEnable(GL_TEXTURE_2D);
         glColor3f(1.0f,1.0f,1.0f);
      } else {
         glColor3f(0,0,0);
      }
      if (render_inside) {
         for (int i = 0 ; i < NUM_CUBE_FACES ; ++i) {
            cube_faces_in[i].Render();
         }
      }
      if (render_outside) {
         for (int i = 0 ; i < NUM_CUBE_FACES ; ++i) {
            cube_faces_out[i].Render();
         }
      }
   }
   
   /// Render edges for visibility
   if (edges_on) {
      glDisable(GL_TEXTURE_2D);
      for (int i = 0 ; i < NUM_CUBE_EDGES ; ++i) {
         cube_edges[i].Render();
      }
   }

}
*/



